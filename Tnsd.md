# Tnsd

## Topology
* Tnsd : Publisher와 Subscriber를 연결시켜주는 doori-service명
* Pub[lisher] : 해당 Topic에 관심있는 Subscriber에게 데이터를 송신하는 Node[s]
* Sub[scriber] : 해당 Topic의 데이터에 관심있는 Node[s]

## StreamProtocol
* Alive : Tnsd에게 살아있음을 알림.
* Notify : Tnsd에 Topic에 관심있다는 것을 알림
* Anwser : Node에 응답 보냄
* Change : Topic관심 있는 Topology가 변경
* Close : 이제는 Topic에 관심을 Off
* Report : 정보성 프로토콜

----

![Alive StreamProtocol](https://github.com/dooripapa/doori-project/blob/master/doori_uml/images/Alive.png)

----

![Notify StreamProtocol](https://github.com/dooripapa/doori-project/blob/master/doori_uml/images/Subscriber_flow.png)

----

![Change StreamProtocol](https://github.com/dooripapa/doori-project/blob/master/doori_uml/images/Publisher_flow.png)

----

![Close StreamProtocol](https://github.com/dooripapa/doori-project/blob/master/doori_uml/images/Close.png)

----

