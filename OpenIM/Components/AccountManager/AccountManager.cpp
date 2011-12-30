//
// Copyright (C) OpenIM developers
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "AccountManager.hpp"

#include "../../Events/AccountEvents.hpp"

#include "../../Spry.hpp"

#include "AccountEditorDialog.hpp"

using namespace Spry;
using namespace Spry::IO;

namespace
{
	void WriteTag(BinaryWriter& writer, unsigned char tag, const wchar_t* value)
	{
		unsigned char len = static_cast<unsigned char>(wcslen(value));

		writer.Write(tag);
		writer.Write(static_cast<unsigned char>(len * sizeof(wchar_t)));
		writer.WriteChars<Encoding::UTF16LE>(value, len);
	}

	void WriteTag(BinaryWriter& writer, unsigned char tag, const ::String& value)
	{
		unsigned char len = static_cast<unsigned char>(value.length()) * sizeof(wchar_t);

		writer.Write(tag);
		writer.Write(len);

		writer.WriteChars<Encoding::UTF16LE>(value.c_str(), static_cast<int>(value.length()));
	}

	void WriteTag(BinaryWriter& writer, unsigned char tag, bool value)
	{
		writer.Write(tag);
		writer.Write((unsigned char)sizeof(value));
		writer.Write(value);
	}

}

AccountManager::AccountManager() :
	_visible(false)
{
	_protocols.push_back(0);

	_bindings.Add<LoadUserProfileEvent>(this, &AccountManager::OnLoadUserProfile);
	_bindings.Add<UnloadUserProfileEvent>(this, &AccountManager::OnUnloadUserProfile);
}

AccountManager::~AccountManager()
{
}

Account* AccountManager::Create(const wchar_t* protocol)
{
	foreach(AccountFactory* factory, _factories)
	{
		if(wcsicmp(factory->GetProtocolName(), protocol) == 0)
			return factory->Create();
	}

	return 0;
}

void AccountManager::Destroy(Account* account)
{
	foreach(AccountFactory* factory, _factories)
	{
		if(wcsicmp(factory->GetProtocolName(), account->GetProtocolName()) == 0)
		{
			factory->Destroy(account);
			break;
		}
	}
}

const wchar_t* AccountManager::GetLostPasswordURL(const wchar_t* protocol)
{
	foreach(AccountFactory* factory, _factories)
	{
		if(wcsicmp(factory->GetProtocolName(), protocol) == 0)
		{
			return factory->GetLostPasswordURL();
		}
	}

	return 0;
}

const wchar_t** AccountManager::GetProtocols()
{
	_protocols.clear();
	foreach(AccountFactory* factory, _factories)
	{
		_protocols.push_back(factory->GetProtocolName());
	}
	_protocols.push_back(0);

	return &_protocols[0];
}

const wchar_t* AccountManager::GetSignupURL(const wchar_t* protocol)
{
	foreach(AccountFactory* factory, _factories)
	{
		if(wcsicmp(factory->GetProtocolName(), protocol) == 0)
		{
			return factory->GetSignupURL();
		}
	}

	return 0;
}

void AccountManager::OnLoadUserProfile(LoadUserProfileEvent& e)
{
	ReadAccounts(e.ProfileDirectory);
	
	int pos = 0;
	foreach(Account* account, _accounts)
	{
		AccountCreateEvent ace;
		ace.Account = account;
		ace.Position = pos;
		ace.Invoke();

		pos++;
	}
}

void AccountManager::OnUnloadUserProfile(UnloadUserProfileEvent& e)
{
	WriteAccounts(e.ProfileDirectory);

	foreach(Account* account, _accounts)
	{
		account->Disconnect();

		Destroy(account);
	}
}

void AccountManager::ReadAccounts(const wchar_t* dir)
{
	String accountFilename = Path::Combine(dir, L"accounts.prefs");
	if(! File::Exists(accountFilename))
		return;

	FileStream stream(accountFilename);
	BinaryReader reader(stream);

	unsigned char version = reader.Read<unsigned char>();
	if(version != 2)
		return;

	while(stream.Position < stream.Length)
	{
		unsigned char tag = reader.Read<unsigned char>();

		// Read protocol in and create an account for it.
		WCharBuffer<UInt8::MaximumValue + 1> buffer;
		unsigned char protocolLength = 0;
		reader >> protocolLength;
		reader.ReadChars<Encoding::UTF16LE>(buffer, protocolLength);

		// FileStream may throw at unexpected EOF.
		AutoPtr<Account> account(Create(buffer));

		try
		{
			// Read the account properties in, and apply them to the
			// newly created account.
			do
			{
				buffer.Zero();

				tag = reader.Read<unsigned char>();
				unsigned char bytesLength = reader.Read<unsigned char>();
				unsigned char wcharLength = bytesLength / sizeof(wchar_t);

				unsigned long nextTagPosition = stream.Position + bytesLength;
				
				switch(tag)
				{
				case UserID:
					reader.ReadChars<Encoding::UTF16LE>(buffer, bytesLength);
					if(account != 0)
						account->SetUserID(buffer);
					break;

				case Password:
					reader.ReadChars<Encoding::UTF16LE>(buffer, bytesLength);
					if(account != 0)
					{
						String password(buffer, wcharLength);

						account->SetPassword(buffer);
						_passwords[account.Get()] = password;
					}
					break;

				case Label:
					reader.ReadChars<Encoding::UTF16LE>(buffer, bytesLength);
					if(account != 0)
						account->SetLabel(buffer);
					break;

				case DisplayName:
					reader.ReadChars<Encoding::UTF16LE>(buffer, bytesLength);
					if(account != 0)
						account->SetDisplayName(buffer);
					break;
/*
				case Autoconnect:
					if(account != 0)
						account->Preferences->ConnectAtStartup = reader.Read<bool>();
					break;

				case Reconnect:
					if(account != 0)
						account->Preferences->Reconnect = reader.Read<bool>();
					break;

				case ReportIdleTime:
					if(account != 0)
						account->Preferences->ReportIdleTime = reader.Read<bool>();
					break;

				case SendTypingNotifications:
					if(account != 0)
						account->Preferences->SendTypingNotifications = reader.Read<bool>();
					break;
*/
				case AccountEnd:
					break;
				}

				stream.Position = nextTagPosition;
			}
			while(tag != AccountEnd);

			if(account.Get() != 0)
			{
				_accounts.push_back(account.Get());
				account.Release();
			}
		}
		catch(...)
		{
			_passwords.erase(account.Get());

			throw;
		}
	}

}

void AccountManager::Register(AccountFactory* factory)
{
	_factories.push_back(factory);
	_protocols.insert(_protocols.begin(), factory->GetProtocolName());
}

void AccountManager::Unregister(AccountFactory* factory)
{
	for(Factories::iterator i = _factories.begin(); i != _factories.end(); ++i)
	{
		if(wcsicmp((*i)->GetProtocolName(), factory->GetProtocolName()) == 0)
		{
			_factories.erase(i);
			break;
		}
	}

	for(Protocols::iterator i = _protocols.begin(); i != _protocols.end(); ++i)
	{
		if(wcsicmp(*i, factory->GetProtocolName()) == 0)
		{
			_protocols.erase(i);
			break;
		}
	}
}

void AccountManager::Show()
{
	if(! _visible)
	{
		_visible = true;

		AccountEditorDialog editor(_accounts, _passwords);
		editor.DoModal();
		
		_visible = false;
	}
}

void AccountManager::WriteAccounts(const wchar_t* dir)
{
	Spry::String accounts = Path::Combine(dir, L"accounts.prefs");
	Spry::String accountsTemp = Path::Combine(dir, L"accounts.prefs.tmp");
	{
		FileStream stream(accountsTemp, FileAccess::Write, FileMode::Create | FileMode::Open);
		BinaryWriter writer(stream);

		unsigned char version = 2;
		writer.Write(version);

		foreach(Account* account, _accounts)
		{
			WriteTag(writer, AccountBegin, account->GetProtocolName());
			
			WriteTag(writer, UserID, account->GetUserID());
			WriteTag(writer, Password, _passwords[account]);
			
			if(account->GetLabel() != 0)
				WriteTag(writer, Label, account->GetLabel());

			if(account->GetDisplayName() != 0)
				WriteTag(writer, DisplayName, account->GetDisplayName());

			/*WriteTag(writer, Autoconnect, account->Preferences->ConnectAtStartup);
			WriteTag(writer, Reconnect, account->Preferences->Reconnect);
			WriteTag(writer, ReportIdleTime, account->Preferences->ReportIdleTime);
			WriteTag(writer, SendTypingNotifications, account->Preferences->SendTypingNotifications);*/
			writer.Write((unsigned char)AccountEnd);
			writer.Write((unsigned char)0);
		}
	}

	File::Copy(accountsTemp, accounts, true);
	File::Delete(accountsTemp);
}
