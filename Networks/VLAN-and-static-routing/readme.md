# Задание

Собрать сеть из четырёх сетей, - A, B, C и D. Сети A и B должны быть созданы с помощью vlan на одном коммутаторе. Маршрутизатор $R_1$ связывает сети A и B между собой и с сетью C. Маршрутизатор $R_2$ связывает сеть C с сетью D.

В каждой из сетей A, B и D подключить хотя бы одно оконечное устройство.

Устройства из каждой сети должны иметь возможность отправлять и получать пакеты устройствам из остальных сетей.

Опционально настроить сервис DHCP на маршрутизаторах, чтобы оконечное оборудование получало IP-адреса автоматически.

На хостах выбрать ip из правильного адресного пространства указать шлюзом(маршрутом) по умолчанию адрес маршрутизатора в L3-домене, создаваемом поверх L2-домена.