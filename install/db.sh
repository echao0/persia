
# create random password
PASSWDDB="$(openssl rand -base64 12)"

# replace "-" with "_" for database username
MAINDB=${persia}

# If /root/.my.cnf exists then it won't ask for root password

    mysql -e "CREATE DATABASE persia /*\!40100 DEFAULT CHARACTER SET utf8 */;"
    mysql -e "CREATE USER persia@localhost IDENTIFIED BY '/Persia/';"
    mysql -e "GRANT ALL PRIVILEGES ON *.* TO 'persia'@'localhost';"
    mysql -e "FLUSH PRIVILEGES;"
    mysql persia < /opt/persia/DataBase/persia.sql


# If /root/.my.cnf doesn't exist then it'll ask for root password
