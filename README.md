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

Hemos accedido a un servidor XMPP en la ci40 (IP 192.168.1.91) desde dos clientes en la máquina virtual (192.168.1.1). El protocolo [XMPP](https://tools.ietf.org/html/rfc6120) funciona sobre TCP, requiere de un establecimiento de sesión y las tramas se confirman a nivel de transporte. Aplicando el filtro *tcp.flags.syn==1* observamos las de sesiones TCP que hemos establecido, desde cada uno de los clientes al servidor.

Empezamos el estudio del protocolo XMPP a partir del paquete 105 del archivo que enlazamos. Analizamos el contenido de las cabeceras y datos de aplicación de las tramas XMPP ignorando sus correspondientes confirmaciones TCP.

En los paquetes 105-107 observamos uno de los establecimientos de sesión TCP de 3 vías. 

A continuación (108), el cliente solicita una conexión XMPP enviando un paquete con el siguiente contenido:

    XMPP Protocol
        XML HEADER VER: 1.0
        STREAM [xmlns="jabber:client" xmlns:stream="http://etherx.jabber.org/streams"]
            xmlns: jabber:client
            xmlns:stream: http://etherx.jabber.org/streams
            version: 1.0 [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute version]
            to: ci40.xmpp.sergio.com [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute to]
            xml:lang: en [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute xml:lang]

(110) El servidor responde con el *stream id*:

    XMPP Protocol
        XML HEADER VER: 1.0
        STREAM [xmlns="jabber:client" xmlns:stream="http://etherx.jabber.org/streams"]
            xmlns: jabber:client
            xmlns:stream: http://etherx.jabber.org/streams
            xml:lang: en [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute xml:lang]
            version: 1.0 [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute version]
            id: 5973707a-4e73-4239-ac4c-27d82ecbd4de [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute id]
            from: ci40.xmpp.sergio.com [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute from]
            FEATURES [UNKNOWN]
                MECHANISMS [xmlns="urn:ietf:params:xml:ns:xmpp-sasl"]
                [Expert Info (Note/Undecoded): Unknown element: features]

(112) El cliente solicita identificarse y envía su nombre de ususario:

    XMPP Protocol
        AUTH [xmlns="urn:ietf:params:xml:ns:xmpp-sasl" mechanism="SCRAM-SHA-1"]
            xmlns: urn:ietf:params:xml:ns:xmpp-sasl
            mechanism: SCRAM-SHA-1
            CDATA: biwsbj1zZXJnaW8scj0yOElFQUFBQUFBQWJsU1ZhQUFBQUFBPT0=

(114) El servidor requiere la contraseña para ese usuario:

    XMPP Protocol
        CHALLENGE [xmlns="urn:ietf:params:xml:ns:xmpp-sasl"]
            [Expert Info (Chat/Response): CHALLENGE]
                [CHALLENGE]
                [Severity level: Chat]
                [Group: Response]
            xmlns: urn:ietf:params:xml:ns:xmpp-sasl
            CDATA: cj0yOElFQUFBQUFBQWJsU1ZhQUFBQUFBPT0xM2NjMmI0YS1lMzY3LTRhNjAtYjY4Ni05MTU0YWVhOGI3NjQscz1ZV1EzTm1FMFpXUXRaVEZqTkMwME9HVXlMV0ptTTJNdFptVXlPVE15WkROaU5UYzQsaT00MDk2

(115) El cliente envía la contraseña:

    XMPP Protocol
        RESPONSE [xmlns="urn:ietf:params:xml:ns:xmpp-sasl"]
            [Expert Info (Chat/Response): RESPONSE]
                [RESPONSE]
                [Severity level: Chat]
                [Group: Response]
            xmlns: urn:ietf:params:xml:ns:xmpp-sasl
            CDATA: Yz1iaXdzLHI9MjhJRUFBQUFBQUFibFNWYUFBQUFBQT09MTNjYzJiNGEtZTM2Ny00YTYwLWI2ODYtOTE1NGFlYThiNzY0LHA9b0NCb0FGUGZzMSt3cTZ1Q3FtcFVOL1liV3hvPQ==

(116) El servidor confirma la identificación:

    XMPP Protocol
        SUCCESS [xmlns="urn:ietf:params:xml:ns:xmpp-sasl"]
            [Expert Info (Chat/Response): SUCCESS]
                [SUCCESS]
                [Severity level: Chat]
                [Group: Response]
            xmlns: urn:ietf:params:xml:ns:xmpp-sasl
            CDATA: dj1HVXBUdUdISnNQT1RtamVpTGFoS2Vkc0xPQjQ9

