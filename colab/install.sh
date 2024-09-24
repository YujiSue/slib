#!/usr/bin/bash
curl -L -o slib4colab.tar.gz https://firebasestorage.googleapis.com/v0/b/publicstorage-3ef6a.appspot.com/o/slib4colab.tar.gz?alt=media&token=b47a7309-9971-43e0-b38c-25fc3b17a365
tar -xfz slib4colab.tar.gz
sudo cp ./slib/lib/* /usr/local/lib
sudo mkdir -p /usr/local/include/slib
sudo cp -r ./slib/include/* /usr/local/include/slib
