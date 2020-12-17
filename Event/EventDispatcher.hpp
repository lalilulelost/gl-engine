// say we have an FPS camera
// it looks around and moves around (on a horizontal plane)
// to look around, we need a 2D vector as "input" to yaw and pitch our camera
// to move around, we need a 2D vector also as "input"
// we want our input-handling class to provide this data,
// but we don't want it coupled to the camera or the camera coupled to it

#include <unordered_map>
#include <typeindex>
#include <functional>



struct Event
{
	std::string eventType;
};


class EventDispatcher
{
public:
	using EventHandler = std::function<void(const Event&)>;
	using HandlerCollection = std::unordered_multimap<std::type_index, EventHandler>;
public:

	template <typename EventSubType>
	void RegisterHandler(const EventHandler& handler)
	{
		eventHandlers.insert(std::pair<std::type_index, EventHandler>(typeid(EventSubType), handler));
	}


	template <typename T>
	void DispatchEvent(const Event& event)
	{
		// I'm not quite sure this cast will work as intended...
		const T& derivedEvent = static_cast<const T&>(event);

		// equal_range gives us begin and end iterators for the range of values under the given key (i.e. event type)
		std::pair<HandlerCollection::iterator, HandlerCollection::iterator> pairs =
			eventHandlers.equal_range(typeid(derivedEvent));


		for (auto iter = pairs.first; iter != pairs.second; ++iter)
		{
			iter->second(derivedEvent);
		}
	}

	static EventDispatcher& Instance()
	{
		return instance;
	}

private:
	HandlerCollection eventHandlers;
	static EventDispatcher instance;

};