#pragma once

#include "socket_like.hpp"
#include "abstract_error.hpp"

namespace Errors {
    class TransmitterError : public AbstractError {
    public:
        explicit TransmitterError(const char* string) : AbstractError(string) {}

        explicit TransmitterError(const std::string& string) : AbstractError(string) {}

        explicit TransmitterError(const std::runtime_error& error) : AbstractError(error) {}

        explicit TransmitterError(const std::string& message, const AbstractError& error) : AbstractError(message, error) {}

        explicit TransmitterError(const char* message, const AbstractError& error) : AbstractError(message, error) {}
    };

    class NoEventsInTransmitter : public TransmitterError {
    public:
        explicit NoEventsInTransmitter() : TransmitterError("No events") {}

        explicit NoEventsInTransmitter(const char* string) : TransmitterError(string) {}

        explicit NoEventsInTransmitter(const std::string& string) : TransmitterError(string) {}

        explicit NoEventsInTransmitter(const std::runtime_error& error) : TransmitterError(error) {}

        explicit NoEventsInTransmitter(const std::string& message, const AbstractError& error) : TransmitterError(message, error) {}

        explicit NoEventsInTransmitter(const char* message, const AbstractError& error) : TransmitterError(message, error) {}

    };
}

namespace Utils {
    enum TransmitterEventType {
        TYPE_1,
        TYPE_2,
        TYPE_3,
        TYPE_4,
    };

    struct TransmitterData {
        int value;
        void* ptr;
    };

    struct TransmitterEvent {
        TransmitterData data{};
        TransmitterEventType type{};
    };

    class Transmitter : public SocketLike {
    public:
        Transmitter();

        void sendInt(int value, TransmitterEventType type) const;

        void sendPtr(void* ptr, TransmitterEventType type) const;

        [[nodiscard]] TransmitterEvent receive() const;

        ~Transmitter() override;

    private:
        class Impl;
        Impl* _pimpl;
    };
}
