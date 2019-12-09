FROM arm32v7/python:buster

RUN apt-get update && apt-get upgrade -y
RUN apt-get install nano python2.7 mariadb-server python-mysqldb git apache2 php -y

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
RUN /etc/init.d/apache2 restart
EXPOSE 80

RUN service mysql restart
RUN service apache2 restart
RUN chmod +x /opt/persia/install/db.sh
RUN /opt/persia/install/db.sh

CMD ["python", "/opt/on/on.py"]

VOLUME ["/etc/mysql", "/var/lib/mysql"]

RUN echo "DONE!"
