FROM arm32v7/python:buster

RUN apt-get update && apt-get upgrade -y
RUN DEBIAN_FRONTEND=noninteractive apt-get install nano python2.7 python-mysqldb git apache2 php php-mysql php-mbstring -y

RUN \
	DEBIAN_FRONTEND=noninteractive apt-get install -y mariadb-server
EXPOSE 3306
 
WORKDIR /opt
RUN git clone https://github.com/echao0/persia.git

RUN mkdir on
WORKDIR /opt/on
COPY on.py /opt/on

WORKDIR /opt/persia
RUN git checkout Docker

WORKDIR /var/www/html
RUN ln -s /opt/persia/web/ web


WORKDIR /etc/apache2/sites-enabled/
RUN sed -i 's,/var/www/html,/var/www/html/web,' 000-default.conf
EXPOSE 80


RUN chmod +x /opt/persia/install/db.sh
#RUN /opt/persia/install/db.sh

CMD ["python2.7", "/opt/on/on.py"]

VOLUME ["/etc/mysql", "/var/lib/mysql"]

WORKDIR /opt/persia/
RUN echo "DONE!"
