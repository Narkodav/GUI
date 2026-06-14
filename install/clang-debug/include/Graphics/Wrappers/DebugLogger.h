#pragma once
#include <chrono>
#include <string>
#include <vector>
#include <shared_mutex>
#include <mutex>
#include "Graphics/HandleTypes/DebugUtilsMessenger.h"

namespace Graphics::Wrappers {
	class DebugLogger {
	public:
		struct Object {
			std::string objectName;
			ObjectType type;
			uint64_t handle;
		};

		struct Message {
			Flags::DebugMessageSeverity::Bits messageSeverity;
			Flags::DebugMessageType messageType;
			std::string message;
			std::string messageIdName;			
			std::chrono::steady_clock::time_point timestamp;
			std::vector<Object> objects;
		};

	private:
		std::vector<Message> m_messages;
		std::array<std::vector<size_t>, 4> m_messageIndicesBySeverity;
		std::array<std::vector<size_t>, 4> m_messageIndicesByType;
		
		mutable std::shared_mutex m_mutex;
	public:
		DebugLogger() = default;
		~DebugLogger() = default;

		DebugLogger(const DebugLogger&) = delete;
		DebugLogger& operator=(const DebugLogger&) = delete;

		DebugLogger(DebugLogger&& other) {
			std::unique_lock lock1(m_mutex);
			std::unique_lock lock2(other.m_mutex);

			m_messages = std::exchange(other.m_messages, std::vector<Message>());
			m_messageIndicesBySeverity = std::exchange(other.m_messageIndicesBySeverity, std::array<std::vector<size_t>, 4>());
			m_messageIndicesByType = std::exchange(other.m_messageIndicesByType, std::array<std::vector<size_t>, 4>());
		}
		DebugLogger& operator=(DebugLogger&& other) {
			if (this == &other) return *this;
			std::unique_lock lock1(m_mutex);
			std::unique_lock lock2(other.m_mutex);

			m_messages = std::exchange(other.m_messages, std::vector<Message>());
			m_messageIndicesBySeverity = std::exchange(other.m_messageIndicesBySeverity, std::array<std::vector<size_t>, 4>());
			m_messageIndicesByType = std::exchange(other.m_messageIndicesByType, std::array<std::vector<size_t>, 4>());
			return *this;
		}

		void logMessage(Flags::DebugMessageSeverity::Bits messageSeverity,
			Flags::DebugMessageType messageType,
			const DebugUtils::CallbackData* pCallbackData);

		template<Flags::DebugMessageSeverity::Bits severity>
		void logMessage(Flags::DebugMessageType messageType, const DebugUtils::CallbackData* pCallbackData) {
			std::unique_lock<std::shared_mutex> lock(m_mutex);

			m_messages.push_back({});
			m_messages.back().messageSeverity = severity;
			m_messages.back().messageType = messageType;
			m_messages.back().messageIdName = pCallbackData->getMessageIdName();
			m_messages.back().message = pCallbackData->getMessage();
			m_messages.back().timestamp = std::chrono::steady_clock::now();
			
			auto objectInfos = pCallbackData->getObjectInfos();
			m_messages.back().objects.resize(objectInfos.size());
			for(size_t i = 0; i < objectInfos.size(); ++i) {
				if(objectInfos[i].hasObjectName())
					m_messages.back().objects[i].objectName = objectInfos[i].getName();
				else m_messages.back().objects[i].objectName = "Unknown";
				m_messages.back().objects[i].type = objectInfos[i].getType();
				m_messages.back().objects[i].handle = objectInfos[i].getHandle();
			}
			
			addMessageByType(m_messages.back(), m_messages.size() - 1);
			m_messageIndicesBySeverity[messageSeverityToIndex<severity>()].push_back(m_messages.size() - 1);
		}

		std::span<const Message> getMessages() const {
			return m_messages;
		};
		const Message& getMessageBySeverity(Flags::DebugMessageSeverity::Bits severity, size_t index) const;
		size_t getMessageBySeveritySize(Flags::DebugMessageSeverity::Bits type) const;
		const Message& getMessageByType(Flags::DebugMessageType::Bits type, size_t index) const;
		size_t getMessageByTypeSize(Flags::DebugMessageType::Bits type) const;

		template<Flags::DebugMessageSeverity::Bits severity>
		const Message& getMessageBySeverity(size_t index) const {
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			return m_messages[m_messageIndicesBySeverity[messageSeverityToIndex<severity>()][index]];
		};

		template<Flags::DebugMessageSeverity::Bits severity>
		size_t getMessageBySeveritySize() const {
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			return m_messageIndicesBySeverity[messageSeverityToIndex<severity>()].size();
		}

		template<Flags::DebugMessageType::Bits type>
		const Message& getMessageByType(size_t index) const {
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			return m_messages[m_messageIndicesByType[messageTypeToIndex<type>()][index]];
		}
		template<Flags::DebugMessageType::Bits type>
		size_t getMessageByTypeSize() const {
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			return m_messageIndicesByType[messageTypeToIndex<type>()].size();
		}

		void dumpMessagesToStream(std::ostream& stream) const {
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			for (const auto& message : m_messages) {
				switch (message.messageSeverity) {
				case Flags::DebugMessageSeverity::Bits::Verbose:
					// Diagnostic messages
					stream << "VERBOSE: " << message.message << '\n';
					dumpObjectDataToStream(stream, message.objects);
					break;

				case Flags::DebugMessageSeverity::Bits::Info:
					// Informational messages like resource creation
					stream << "INFO: " << message.message << '\n';
					dumpObjectDataToStream(stream, message.objects);
					break;

				case Flags::DebugMessageSeverity::Bits::Warning:
					// Warning messages like use of deprecated functions
					stream << "WARNING: " << message.message << '\n';
					dumpObjectDataToStream(stream, message.objects);
					break;

				case Flags::DebugMessageSeverity::Bits::Error:
					// Error messages for invalid behavior
					stream << "ERROR: " << message.message << '\n';
					dumpObjectDataToStream(stream, message.objects);
					break;

				default:
					stream << "UNKNOWN SEVERITY: " << message.message << '\n';
					dumpObjectDataToStream(stream, message.objects);
					break;
				}
			}
		}

		void reset() {
			std::unique_lock<std::shared_mutex> lock(m_mutex);
			m_messages.clear();
			m_messageIndicesBySeverity.fill({});
			m_messageIndicesByType.fill({});
		}

	private:
		
		void dumpObjectDataToStream(std::ostream& stream, const std::vector<Object>& objects) const {
			for(size_t i = 0; i < objects.size(); ++i) {
				stream << "	OBJECT[" << i << "]:\n";
				stream << "	NAME: " << objects[i].objectName << "\n";
				stream << "	TYPE: " << ObjectTypeManager::getObjectTypeName(objects[i].type) << "\n";
				stream << "	HANDLE: " << objects[i].handle << "\n";
			}
		}

		void addMessageBySeverity(Message& message, size_t index);
		void addMessageByType(Message& message, size_t index);

		static size_t messageTypeToIndex(Flags::DebugMessageType::Bits messageType);
		static size_t messageSeverityToIndex(Flags::DebugMessageSeverity::Bits messageSeverity);

		template<Flags::DebugMessageType::Bits messageType>
		static size_t messageTypeToIndex() {
			if constexpr (messageType == Flags::DebugMessageType::Bits::General) {
				return 0;
			}
			else if constexpr (messageType == Flags::DebugMessageType::Bits::Validation) {
				return 1;
			}
			else if constexpr (messageType == Flags::DebugMessageType::Bits::Performance) {
				return 2;
			}
			else if constexpr (messageType == Flags::DebugMessageType::Bits::DeviceAddressBinding) {
				return 3;
			}
			else {
				static_assert(false, "Invalid message type");
			}
		}

		template<Flags::DebugMessageSeverity::Bits messageSeverity>
		static size_t messageSeverityToIndex() {
			if constexpr (messageSeverity == Flags::DebugMessageSeverity::Bits::Verbose) {
				return 0;
			}
			else if constexpr (messageSeverity == Flags::DebugMessageSeverity::Bits::Info) {
				return 1;
			}
			else if constexpr (messageSeverity == Flags::DebugMessageSeverity::Bits::Warning) {
				return 2;
			}
			else if constexpr (messageSeverity == Flags::DebugMessageSeverity::Bits::Error) {
				return 3;
			}
			else {
				static_assert(false, "Invalid message severity");
			}
		}
	};
}

