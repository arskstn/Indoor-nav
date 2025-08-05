#include <simplebluez/Exceptions.h>

namespace SimpleBluez {

namespace Exception {

ServiceNotFoundException::ServiceNotFoundException(const std::string& service) {
    _message = "Service " + service +" not found.";
}

const char* ServiceNotFoundException::what() const noexcept { return _message.c_str(); }

CharacteristicNotFoundException::CharacteristicNotFoundException(const std::string& characteristic) {
    _message = "Characteristic " + characteristic + " not found.";
}

const char* CharacteristicNotFoundException::what() const noexcept { return _message.c_str(); }

DescriptorNotFoundException::DescriptorNotFoundException(const std::string& descriptor) {
    _message = "Descriptor " + descriptor + " not found.";
}

const char* DescriptorNotFoundException::what() const noexcept { return _message.c_str(); }

}  // namespace Exception

}  // namespace SimpleBluez
