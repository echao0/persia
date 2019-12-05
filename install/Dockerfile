FROM arm32v7/python:buster

RUN apt-get update && apt-get upgrade -y

RUN apt-get install nano -y

WORKDIR /opt

RUN mkdir on

WORKDIR /opt/on

COPY on.py /opt/on

WORKDIR /opt

RUN apt-get install git -y

RUN git clone https://github.com/echao0/persia.git

EXPOSE 8618

RUN /opt/persia/install/install.sh

RUN /etc/init.d/mysql stop

RUN /etc/init.d/mysql start

#RUN mariadb persia < /opt/persia/DataBase/persia.sql

CMD ["python", "/opt/on/on.py"]

RUN echo "DONE!"
