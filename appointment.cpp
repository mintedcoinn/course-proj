#include "appointment.h"

Appointment::Appointment() {}
Appointment::~Appointment(){}
bool Appointment::operator==(const Appointment& other) const{
    return (this->customer == other.customer && this->date == other.date && this->executer == other.executer && this->service_name == other.service_name &&this->id == other.id);
}
bool Appointment::operator !=(const Appointment& other) const{
    return !(*this==other);
}
Appointment& Appointment::operator =(const Appointment& other){
    if (this != &other)
    {
        service_name = other.service_name;
        customer = other.customer;
        executer = other.executer;
        date = other.date;
        id = other.id;
    }
    return *this;
}
