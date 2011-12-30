#ifndef SPRY_SERIALIZATION_BINARYSERIALIZER_HPP
#define SPRY_SERIALIZATION_BINARYSERIALIZER_HPP

#include "../IO/BinaryReader.hpp"
#include "../IO/BinaryWriter.hpp"
#include "../IO/Stream.hpp"

#include "../Metaprogramming/IntToType.hpp"
#include "../Metaprogramming/TypeTraits.hpp"

#include "Serializable.hpp"

namespace Spry
{
namespace Serialization
{
namespace Internal
{
	// Maps operator& to corresponding Read<T> member function.
	//
	template<typename Reader>
	class BinaryReaderContext
	{
	public:
		BinaryReaderContext(Reader& r) :
			_reader(r)
		{
		}

		template<typename T>
		BinaryReaderContext<Reader>& operator&(T& obj)
		{
			obj = _reader.Read<T>();

			return *this;
		}
        
		void Skip(int bytes)
		{
			_reader.BaseStream.Skip(bytes);
		}

	public:
		static const bool Reading = true;

	private:
		Reader _reader;
	};

	// Maps operator& to corresponding Write<T> member function.
	//
	template<typename Writer>
	class BinaryWriterContext
	{
	public:
		BinaryWriterContext(Writer& r) :
			_writer(r)
		{
		}

		template<typename T>
		BinaryWriterContext<Writer>& operator&(T& obj)
		{
			_writer.Write<T>(obj);

			return *this;
		}
        
		void Skip(int)
		{
		}

	public:
		static const bool Reading = false;

	private:
		Writer _writer;
	};
}

	// Forward declaration.
	template<typename Context, typename T>
	void Serialize(Context& context, T& object);

	class BinarySerializer
	{
	public:
		explicit BinarySerializer(Spry::IO::Stream& stream) :
			BaseStream(stream)
		{
		}

		// Reads the specified object from the attached stream using
		// the class template arguments for Reader and Writer.
		//
		template<typename T>
		void Deserialize(T& obj)
		{
			Spry::IO::BinaryReader reader(BaseStream);
			
			Internal::BinaryReaderContext<Spry::IO::BinaryReader> context(reader);
			DoSerialize(context, const_cast<Spry::Metaprogramming::TypeTraits<T>::NonConstType&>(obj));
		}

		// Writes the specified object out to the attached stream using
		// the class template arguments for Reader and Writer.
		//
		template<typename T>
		void Serialize(T& obj)
		{
			Spry::IO::BinaryWriter writer(BaseStream);

			Internal::BinaryWriterContext<Spry::IO::BinaryWriter> context(writer);
			DoSerialize(context, const_cast<Spry::Metaprogramming::TypeTraits<T>::NonConstType&>(obj));
		}

		Spry::IO::Stream& BaseStream;

	private:
		// Deduces whether the object being serialized is a fundamental type.
		// 
		template<typename Context, typename T>
		void DoSerialize(Context& context, T& object)
		{
			DoSerializeImpl(context, object, Spry::Metaprogramming::IntToType<Spry::Metaprogramming::TypeTraits<T>::IsFundamental>());
		}

		// Serializes a fundamental type.
		//
		template<typename Context, typename T>
		void DoSerializeImpl(Context& context, T& object, Spry::Metaprogramming::IntToType<true>)
		{
			context & object;
		}

		// Handle user-defined types. They can be versioned, marked
		// serializable, or handled by a free-standing function.
		//
		template<typename Context, typename T>
		void DoSerializeImpl(Context& context, T& object, Spry::Metaprogramming::IntToType<false>)
		{
			DoSerializeUserType(context, object);
		}

		template<typename Context, typename T>
		void DoSerializeUserType(Context& context, Serializable<T>& object)
		{
			object.Serialize(context);
		}

		template<typename Context, typename T>
		void DoSerializeUserType(Context& context, T& object)
		{
			Spry::Serialization::Serialize(context, object);
		}
	};
}
}

#endif