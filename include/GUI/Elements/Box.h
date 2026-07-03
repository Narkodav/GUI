// #pragma once
// #include "GUI/Instance.h"
// #include "GUI/Elements/Element.h"

// #include <glm/glm.hpp>
// #include <vector>
// #include <cstdint>

// namespace GUI::Elements
// {
// 	class Box;
// }

// namespace GUI {
// 	enum class BoxEvent {
// 		SizeChanged,
// 		SizeChangedX,
// 		SizeChangedY,
// 		PositionChanged,
// 		PositionChangedX,
// 		PositionChangedY,
//         Count,
// 	};

// 	struct BoxEventPolicy {
// 		using Type = BoxEvent;

// 		template<Type T>
// 		struct Trait {};

// 		template <Type T>
// 		static void handleError(std::exception_ptr) {
// 			std::cerr << "[GUI] Exception in Box event callback: " << Trait<T>::s_name << std::endl;
// 		}
// 	};
// }

// template<>
// struct GUI::BoxEventPolicy::Trait<GUI::BoxEvent::SizeChanged> {
// 	using Signature = void(const glm::ivec2& newSize);
// 	static inline const std::string_view s_name = "SizeChanged";
// };

// template<>
// struct GUI::BoxEventPolicy::Trait<GUI::BoxEvent::SizeChangedX> {
// 	using Signature = void(int newSizeX);
// 	static inline const std::string_view s_name = "SizeChangedX";
// };

// template<>
// struct GUI::BoxEventPolicy::Trait<GUI::BoxEvent::SizeChangedY> {
// 	using Signature = void(int newSizeY);
// 	static inline const std::string_view s_name = "SizeChangedY";
// };

// template<>
// struct GUI::BoxEventPolicy::Trait<GUI::BoxEvent::PositionChanged> {
// 	using Signature = void(const glm::ivec2& newPosition);
// 	static inline const std::string_view s_name = "PositionChanged";
// };

// template<>
// struct GUI::BoxEventPolicy::Trait<GUI::BoxEvent::PositionChangedX> {
// 	using Signature = void(int newPositionX);
// 	static inline const std::string_view s_name = "PositionChangedX";
// };

// template<>
// struct GUI::BoxEventPolicy::Trait<GUI::BoxEvent::PositionChangedY> {
// 	using Signature = void(int newPositionY);
// 	static inline const std::string_view s_name = "PositionChangedY";
// };

// namespace GUI::Elements
// {
//     class Box : public Element {
//         GUI_DECLARE_OBJECT(Box, Element)
// 		GUI_DECLARE_OBJECT_EVENT_SYSTEM(Box, Element)

// 	public:
// 		virtual ~Box() = default;

// 		Box& setPosition(const glm::ivec2& position) {
// 			if(getPosition() != position) {
// 				onSetPosition(position);
// 				m_events.emit<BoxEvent::PositionChanged>(position);
// 			}
// 			return *this;
// 		}
		
// 		Box& setPositionX(int positionX) {
// 			if(getPositionX() != positionX) {
// 				onSetPositionX(positionX);
// 				m_events.emit<BoxEvent::PositionChangedX>(positionX);
// 			}
// 			return *this;
// 		}

// 		Box& setPositionY(int positionY) {
// 			if(getPositionY() != positionY) {
// 				onSetPositionY(positionY);
// 				m_events.emit<BoxEvent::PositionChangedY>(positionY);
// 			}
// 			return *this;
// 		}

// 		Box& setSize(const glm::ivec2& size) {
// 			if(getSize() != size) {
// 				onSetSize(size);
// 				m_events.emit<BoxEvent::SizeChanged>(size);
// 			}
// 			return *this;
// 		}

// 		Box& setSizeX(int sizeX) {
// 			if(getSizeX() != sizeX) {
// 				onSetSizeX(sizeX);
// 				m_events.emit<BoxEvent::SizeChangedX>(sizeX);
// 			}
// 			return *this;
// 		}

// 		Box& setSizeY(int sizeY) {
// 			if(getSizeY() != sizeY) {
// 				onSetSizeY(sizeY);
// 				m_events.emit<BoxEvent::SizeChangedY>(sizeY);
// 			}
// 			return *this;
// 		}

// 		virtual glm::ivec2 getPosition() const = 0;
// 		virtual glm::ivec2 getSize() const = 0;

// 		virtual int getPositionX() const = 0;
// 		virtual int getPositionY() const = 0;
// 		virtual int getSizeX() const = 0;
// 		virtual int getSizeY() const = 0;

//         bool pointerIsInside(const Pointer& pointer) const {
// 			const auto& pos = getPosition();
// 			const auto& size = getSize();
//             if (pointer.position.x > pos.x && pointer.position.x < pos.x + size.x &&
//                 pointer.position.y > pos.y && pointer.position.y < pos.y + size.y)
//                 return true;
//             return false;
//         }

//         virtual const Box& record(GUI::InstanceInterface& instance) const { return record(instance, glm::ivec2(0, 0)); }
//         virtual const Box& record(GUI::InstanceInterface& instance, const glm::ivec2& offset) const {
//             if(!onRecord(instance, positionOffset)) return *this;
//             for (const auto& child : m_children) {
// 				if(child->isConvertible(GUI::Elements::Box::typeIdStatic())) 
// 					child->record(instance, positionOffset + getPosition()); 
// 				else child->record(instance); 
// 			}
//             return *this;
//         }

// 	protected:
// 		virtual bool onRecord(InstanceInterface& instance) { return true; }
// 		virtual bool onRecord(InstanceInterface& instance, const glm::ivec2& offset) const = 0;

// 		virtual void onSetSize(const glm::ivec2& size) = 0;
// 		virtual void onSetSizeX(int sizeX) = 0;
// 		virtual void onSetSizeY(int sizeY) = 0;
// 		virtual void onSetPosition(const glm::ivec2& position) = 0;
// 		virtual void onSetPositionX(int positionX) = 0;
// 		virtual void onSetPositionY(int positionY) = 0;
// 	};
// }