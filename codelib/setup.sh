cd $HOME
sudo chmod a+rw /dev/ttyACM0
cd arduino/libraries/
ls RTClib || git clone https://github.com/adafruit/RTClib.git
ls /tmp/a || ln -s $HOME/ardhome/codelib /tmp/a
