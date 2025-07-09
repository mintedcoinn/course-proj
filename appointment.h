#ifndef APPOINTMENT_H
#define APPOINTMENT_H
#include <QString>
class Appointment
{
public:
    QString  service_name;
    QString  customer;
    QString  executer;
    int date;
    int id;

    Appointment();
    ~Appointment();
    Appointment& operator=(const Appointment& other);
    bool operator==(const Appointment& other) const;
    bool operator!=(const Appointment& other) const;
};

#endif // APPOINTMENT_H
