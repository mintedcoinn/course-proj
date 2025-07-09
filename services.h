#ifndef SERVICES_H
#define SERVICES_H
#include <QString>
class Services
{
public:
    QString  service_name;
    int price;
    int duration;
    int id;

    Services();
    ~Services();
    Services& operator= (const Services& other);
    bool operator==(const Services& other)const;
    bool operator!=(const Services& other)const;
};

#endif // SERVICES_H
