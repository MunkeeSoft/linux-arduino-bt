#!/bin/sh
cd /home/garrett/bt

temp=$(./bt.py -q 07:12:05:16:70:12 temp)
if [  "`echo \"$temp >= -400\" | bc`" -ne "0" ]; then
	echo "Temperature: $temp F"
	curl -i -q -X POST -k -d '{"accessKey":"1wF7arMxzFBNEXUZ106QAn71tzRkkahI","streamName":"temp_line","point":"'$temp'"}' https://www.leftronic.com/customSend/

	if [ "`echo \"$temp < 55\" | bc`" -ne "0" ]; then
		curl -i -q -X POST -k -d '{"accessKey":"1wF7arMxzFBNEXUZ106QAn71tzRkkahI","streamName":"temp_stoplight","point":"50"}' https://www.leftronic.com/customSend/
	elif [ "`echo \"$temp < 35\" | bc`" -ne "0" ]; then
		curl -i -q -X POST -k -d '{"accessKey":"1wF7arMxzFBNEXUZ106QAn71tzRkkahI","streamName":"temp_stoplight","point":"100"}' https://www.leftronic.com/customSend/
	fi
fi

humid=$(./bt.py -q 07:12:05:16:70:12 humid)
if [ "`echo \"$humid >= 0\" | bc`" -ne "0" ]; then
	echo "Humidity: $humid %"
	curl -i -q -X POST -k -d '{"accessKey":"1wF7arMxzFBNEXUZ106QAn71tzRkkahI","streamName":"humid_line","point":"'$humid'"}' https://www.leftronic.com/customSend/

	if [ "`echo \"$humid < 50\" | bc`" -ne "0" ]; then
		curl -i -q -X POST -k -d '{"accessKey":"1wF7arMxzFBNEXUZ106QAn71tzRkkahI","streamName":"humid_stoplight","point":"50"}' https://www.leftronic.com/customSend/
	elif [ "`echo \"$humid < 15\" | bc`" -ne "0" ]; then
		curl -i -q -X POST -k -d '{"accessKey":"1wF7arMxzFBNEXUZ106QAn71tzRkkahI","streamName":"humid_stoplight","point":"100"}' https://www.leftronic.com/customSend/
	fi
fi

light=$(./bt.py -q 07:12:05:16:70:12 light)
echo "Light: $light %"
curl -i -q -X POST -k -d '{"accessKey":"1wF7arMxzFBNEXUZ106QAn71tzRkkahI","streamName":"light_line","point":"'$light'"}' https://www.leftronic.com/customSend/

if [ "`echo \"$light < 50\" | bc`" -ne "0" ]; then
	curl -i -q -X POST -k -d '{"accessKey":"1wF7arMxzFBNEXUZ106QAn71tzRkkahI","streamName":"light_stoplight","point":"50"}' https://www.leftronic.com/customSend/
elif [ "`echo \"$light < 15\" | bc`" -ne "0" ]; then
	curl -i -q -X POST -k -d '{"accessKey":"1wF7arMxzFBNEXUZ106QAn71tzRkkahI","streamName":"light_stoplight","point":"100"}' https://www.leftronic.com/customSend/
fi
