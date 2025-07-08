#ifndef APPOINTMENT_H
#define APPOINTMENT_H

class Appointment
{
public:
    QString  service_name;
    QString  customer;
    QString  executer;
    int date;
    Appointment();
    ~Appointment();

};

#endif // APPOINTMENT_H
