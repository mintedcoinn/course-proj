#include "services.h"

Services::Services() {}
Services::~Services() {}
bool Services::operator==(const Services& other) const{
    return ( this->duration == other.duration && this->price == other.price && this->service_name == other.service_name &&this->id == other.id);
}
bool Services::operator!=(const Services& other) const{
    return !(*this==other);
}
Services& Services::operator=(const Services& other){
    if (this != &other)
    {
        service_name = other.service_name;
        price = other.price;
        duration = other.duration;
        id = other.id;
    }
    return *this;
}
