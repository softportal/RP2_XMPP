# RP2_XMPP

## Alumnos

Sergio Semedi Barranco
Lucas Segarra Fernández

## Memoria

**Ejercicio. Qué puertos están abiertos?**

    root@myCi40:~# netstat -tln | grep 52*

tcp        0      0 0.0.0.0:5222            0.0.0.0:*               LISTEN
tcp        0      0 0.0.0.0:5269            0.0.0.0:*               LISTEN
tcp        0      0 0.0.0.0:53              0.0.0.0:*               LISTEN
tcp        0      0 :::5222                 :::*                    LISTEN
tcp        0      0 :::5354                 :::*                    LISTEN
tcp        0      0 :::5269                 :::*                    LISTEN
tcp        0      0 :::53                   :::*                    LISTEN

**Ejercicio.Cargar el módulo bosh y observar qué puertos están abiertos**

tcp        0      0 0.0.0.0:5280            0.0.0.0:*               LISTEN      
tcp        0      0 0.0.0.0:5222            0.0.0.0:*               LISTEN      
tcp        0      0 0.0.0.0:5269            0.0.0.0:*               LISTEN      
tcp        0      0 0.0.0.0:53              0.0.0.0:*               LISTEN      
tcp        0      0 :::5280                 :::*                    LISTEN      
tcp        0      0 :::5222                 :::*                    LISTEN      
tcp        0      0 :::5354                 :::*                    LISTEN      
tcp        0      0 :::5269                 :::*                    LISTEN      
tcp        0      0 :::53                   :::*                    LISTEN 

A diferencia del ejercicio anterior, al cargar el módulo bosh, observamos que se abre el puerto 5280

**Ejercicio. Documentarse en https://prosody.im/doc/configure y registrar al menos dos usuarios.**

       prosodyctl adduser sergio@ci40.xmpp.lucas.com
       prosodyctl adduser lucas@ci40.xmpp.lucas.com

**Ejercicio.  Arrancar profanity en un par de terminales, enviar mensajes y capturarlos y estudiarlos por medio de wireshark**

El [siguiente enlace](https://github.com/softportal/Bin/blob/master/xmpp.out) es a la salida obtenida con la herramienta *tcpdump*.

Hemos accedido a un servidor XMPP en la ci40 (IP 192.168.1.91) desde dos clientes en la máquina virtual (192.168.1.1).Aplicando el filtro *tcp.flags.syn==1* observamos el número de sesiones TCP que hemos establecido, desde cada uno de los clientes al servidor.

En los paquetes 105-107 observamos uno de los establecimientos de sesión TCP de 3 vías. A continuación, el cliente envía un segmento TCP con la siguiente información XMPP:

      XML HEADER VER: 1.0
      STREAM [xmlns="jabber:client" xmlns:stream="http://etherx.jabber.org/streams"]

Dicho segmento es confirmado nivel TCP por el servidor 
    


