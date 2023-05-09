cd $HOME
sudo chmod a+rw /dev/ttyACM0 && (echo "sudo chmod a+rw /dev/ttyACM0")
cd arduino/libraries/
( ls RTClib > /dev/null 2>&1 ) || ( echo "Installing ada rtc" && git clone https://github.com/adafruit/RTClib.git )
( ls /tmp/a > /dev/null 2>&1 ) || ( echo "Creating /tmp/a symlink" && ln -s $HOME/ardhome/codelib /tmp/a )
