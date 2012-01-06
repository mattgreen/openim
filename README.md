### Introduction
Many people experiment in college; I experimented with software architecture. Over the course of a few years in college I hacked on a component-based GUI Win32 C++ instant messaging client that interoperates with AOL's proprietary OSCAR protocol.

### Why?

At the time, there were few quality IM clients on the Windows platform.
Most were either unable to use all of the features of AIM, or had
subpar UI. (Unfortunately, this situation hasn't changed much.)

### What makes this interesting?
OpenIM uses a publish/subscribe architecture to minimize coupling
between components. This results in components that are
smaller, easier to reason about, and test. For example, the AIM protocol
component has no knowledge of any UI; it merely responds to method calls
and sends out events (such as AccountConnectedEvent) as they occur. The EventBus is responsible for distributing Events to registered components. Partitioning features along component boundaries makes development incredibly pleasant.

(Also, the use of <a href="http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern">CRTP</a> in <a href="https://github.com/mattgreen/openim/blob/master/OpenIM/Events/AccountEvents.hpp">AccountEvents.hpp</a> should induce the Inception theme in
your head.)

### What became of it?
I graduated, and became more social. Regardless, I thoroughly enjoyed the late nights spent hacking on it, and do
believe it is possible to make nice software, even in C++.
