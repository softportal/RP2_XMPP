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

(112) El cliente envía una **solicitud de conexión SASL** *certificada*(traducido del inglés *qualified*) con el *namespace*, incluyendo un valor válido para el parámetro mecanismo y la *initial response* en términos SASL en el *XML character data*:

    XMPP Protocol
        AUTH [xmlns="urn:ietf:params:xml:ns:xmpp-sasl" mechanism="SCRAM-SHA-1"]
            xmlns: urn:ietf:params:xml:ns:xmpp-sasl
            mechanism: SCRAM-SHA-1
            CDATA: biwsbj1zZXJnaW8scj0yOElFQUFBQUFBQWJsU1ZhQUFBQUFBPT0=

(114) El servidor **prueba (challenges) al cliente** enviando una respuesta *certificada* con el *namespace* :

    XMPP Protocol
        CHALLENGE [xmlns="urn:ietf:params:xml:ns:xmpp-sasl"]
            [Expert Info (Chat/Response): CHALLENGE]
                [CHALLENGE]
                [Severity level: Chat]
                [Group: Response]
            xmlns: urn:ietf:params:xml:ns:xmpp-sasl
            CDATA: cj0yOElFQUFBQUFBQWJsU1ZhQUFBQUFBPT0xM2NjMmI0YS1lMzY3LTRhNjAtYjY4Ni05MTU0YWVhOGI3NjQscz1ZV1EzTm1FMFpXUXRaVEZqTkMwME9HVXlMV0ptTTJNdFptVXlPVE15WkROaU5UYzQsaT00MDk2

(115) El cliente **responde a la prueba**:

    XMPP Protocol
        RESPONSE [xmlns="urn:ietf:params:xml:ns:xmpp-sasl"]
            [Expert Info (Chat/Response): RESPONSE]
                [RESPONSE]
                [Severity level: Chat]
                [Group: Response]
            xmlns: urn:ietf:params:xml:ns:xmpp-sasl
            CDATA: Yz1iaXdzLHI9MjhJRUFBQUFBQUFibFNWYUFBQUFBQT09MTNjYzJiNGEtZTM2Ny00YTYwLWI2ODYtOTE1NGFlYThiNzY0LHA9b0NCb0FGUGZzMSt3cTZ1Q3FtcFVOL1liV3hvPQ==

(117) El servidor notifica el éxito del *handshake*, :

    XMPP Protocol
        SUCCESS [xmlns="urn:ietf:params:xml:ns:xmpp-sasl"]
            [Expert Info (Chat/Response): SUCCESS]
                [SUCCESS]
                [Severity level: Chat]
                [Group: Response]
            xmlns: urn:ietf:params:xml:ns:xmpp-sasl
            CDATA: dj1HVXBUdUdISnNQT1RtamVpTGFoS2Vkc0xPQjQ9

Una vez recibida la notificación de éxito, el cliente **tiene** que iniciar un nuevo *stream* sobre la conexión TCP existente enviando una nueva *cabecera stream inicial* al servidor.

(118) Cliente a servidor:

    XMPP Protocol
        XML HEADER VER: 1.0
        STREAM [xmlns="jabber:client" xmlns:stream="http://etherx.jabber.org/streams"]
            xmlns: jabber:client
            xmlns:stream: http://etherx.jabber.org/streams
            version: 1.0 [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute version]
                    [Unknown attribute version]
                    [Severity level: Note]
                    [Group: Undecoded]
            to: ci40.xmpp.sergio.com [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute to]
                    [Unknown attribute to]
                    [Severity level: Note]
                    [Group: Undecoded]
            xml:lang: en [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute xml:lang]
                    [Unknown attribute xml:lang]
                    [Severity level: Note]
                    [Group: Undecoded]
                    
Cuando ha recibido la nueva *cabecera stream inicial* el servidor **tiene** que responder una nueva *cabecera stream* para la que **tiene** que generar un nuevo *stream ID*. También **tiene** que enviar las características del *stream*.

(119) Servidor a cliente:

    XMPP Protocol
        XML HEADER VER: 1.0
        STREAM [xmlns="jabber:client" xmlns:stream="http://etherx.jabber.org/streams"]
            xmlns: jabber:client
            xmlns:stream: http://etherx.jabber.org/streams
            xml:lang: en [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute xml:lang]
                    [Unknown attribute xml:lang]
                    [Severity level: Note]
                    [Group: Undecoded]
            version: 1.0 [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute version]
                    [Unknown attribute version]
                    [Severity level: Note]
                    [Group: Undecoded]
            id: 184733da-05ef-4c8c-ac3c-5763f46a0002 [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute id]
                    [Unknown attribute id]
                    [Severity level: Note]
                    [Group: Undecoded]
            from: ci40.xmpp.sergio.com [UNKNOWN ATTR]
                [Expert Info (Note/Undecoded): Unknown attribute from]
                    [Unknown attribute from]
                    [Severity level: Note]
                    [Group: Undecoded]
            FEATURES [UNKNOWN]
                VER [xmlns="urn:xmpp:features:rosterver"]
                BIND [xmlns="urn:ietf:params:xml:ns:xmpp-bind"]
                SESSION [xmlns="urn:ietf:params:xml:ns:xmpp-session"]
                C [xmlns="http://jabber.org/protocol/caps" node="http://prosody.im" hash="sha-1" ver="k07nuHawZqmndRtf3ZfBm54FwL0="]
                [Expert Info (Note/Undecoded): Unknown element: features]
                    [Unknown element: features]
                    [Severity level: Note]
                    [Group: Undecoded]

Después de identificarse con un servidor, un cliente **tiene** que vincularse un recurso específico del *stream* para que el servidor pueda direccionarlo correctamente. (**Tiene** que haber un recurso XMPP asociado al *JID* puro del cliente)

(120) Cliente a servidor:

    XMPP Protocol
        IQ [id="_xmpp_bind1" type="set"]
            id: _xmpp_bind1
            type: set
            BIND [xmlns="urn:ietf:params:xml:ns:xmpp-bind" resource="profanity"]
                xmlns: urn:ietf:params:xml:ns:xmpp-bind
                resource: profanity
                REQUIRED [UNKNOWN]
                    [Expert Info (Note/Undecoded): Unknown element: required]
                        [Unknown element: required]
                        [Severity level: Note]
                        [Group: Undecoded]
        [Response In: 121]

Cuando el servidor genera un recurso XMPP para el cliente, le envía un *IQ Stanza* de tipo *RESULT*, que **tiene** que incluir un elemento <jid/> con el JID completo para el recurso conectado como determina el servidor.

(121) Servidor a cliente:

    XMPP Protocol
        IQ [id="_xmpp_bind1" type="result"]
            id: _xmpp_bind1
            type: result
            BIND [xmlns="urn:ietf:params:xml:ns:xmpp-bind" jid="sergio@ci40.xmpp.sergio.com/profanity"]
                xmlns: urn:ietf:params:xml:ns:xmpp-bind
                jid: sergio@ci40.xmpp.sergio.com/profanity
        [Request In: 120]
        
Una vez el cliente y el servidor han terminado la negociación *stream*, cualquiera de las partes puede enviar *XML stanzas*.

El cliente enva un *IQ Stanza* de tipo *set* para establecer un nuevo valor con el *id* \_xmpp_session1.

(122) Cliente a servidor:

    XMPP Protocol
        IQ [id="_xmpp_session1" type="set"]
            id: _xmpp_session1
            type: set
            SESSION [xmlns="urn:ietf:params:xml:ns:xmpp-session"]
                xmlns: urn:ietf:params:xml:ns:xmpp-session
        [Response In: 123]

El servidor responde a la petición con un *IQ Stanza* de tipo *result* con el mismo *id*.

(123) Servidor a cliente:

    XMPP Protocol
        IQ [id="_xmpp_session1" type="result" to="sergio@ci40.xmpp.sergio.com/profanity"]
            id: _xmpp_session1
            type: result
            to: sergio@ci40.xmpp.sergio.com/profanity
        [Request In: 122]

El cliente hace una petición *IQ Stanza* de tipo *get* solicitando información, con el *id* rooster.

(124) Cliente a servidor:

    XMPP Protocol
        IQ [id="roster" type="get"]
            id: roster
            type: get
            QUERY [xmlns="jabber:iq:roster"]
                xmlns: jabber:iq:roster
        [Response In: 125]

El servidor responde a la petición con el mismo *id* y la información solicitada.

(125) Servidor a cliente:

    XMPP Protocol
        IQ [id="roster" type="result" to="sergio@ci40.xmpp.sergio.com/profanity"]
            id: roster
            type: result
            to: sergio@ci40.xmpp.sergio.com/profanity
            QUERY [xmlns="jabber:iq:roster" ver="1"]
                xmlns: jabber:iq:roster
                ver: 1
        [Request In: 124]
        


(126) El cliente hace una petición *IQ Stanza* de tipo *get* solicitando información, con el *id* bookmark_init_request Cliente a servidor:

    XMPP Protocol
        IQ [id="bookmark_init_request" type="get"]
            id: bookmark_init_request
            type: get
            QUERY [xmlns="jabber:iq:private"] [UNKNOWN]
                xmlns: jabber:iq:private
                STORAGE [xmlns="storage:bookmarks"]
                    xmlns: storage:bookmarks
                [Expert Info (Note/Undecoded): Unknown element: query]
                    [Unknown element: query]
                    [Severity level: Note]
                    [Group: Undecoded]
        [Response In: 127]

(127) Respuesta *IQ Stanza* tipo *result* servidor a cliente, con el mismo *id*:

    XMPP Protocol
        IQ [id="bookmark_init_request" type="result" to="sergio@ci40.xmpp.sergio.com/profanity"]
            id: bookmark_init_request
            type: result
            to: sergio@ci40.xmpp.sergio.com/profanity
            QUERY [xmlns="jabber:iq:private"] [UNKNOWN]
                xmlns: jabber:iq:private
                STORAGE [xmlns="storage:bookmarks"]
                    xmlns: storage:bookmarks
                [Expert Info (Note/Undecoded): Unknown element: query]
                    [Unknown element: query]
                    [Severity level: Note]
                    [Group: Undecoded]
        [Request In: 126]

El cliente enva una *Presence Stanza* sin atributo *to* que el servidor redifunde a todas las entidades suscritas. 

(128) Cliente a servidor:

    XMPP Protocol
        PRESENCE [id="prof_presence_1"]
            id: prof_presence_1
            CAPS [hash="sha-1" node="http://www.profanity.im"]
                xmlns: http://jabber.org/protocol/caps
                hash: sha-1
                node: http://www.profanity.im
                ver: jra5S1tICxzuMGVnu86Tv5flMBU=
                
En este punto la única entidad suscrita es el propio cliente.

(129) Servidor a cliente:

    XMPP Protocol
        PRESENCE [id="prof_presence_1"]
            from: sergio@ci40.xmpp.sergio.com/profanity
            id: prof_presence_1
            CAPS [hash="sha-1" node="http://www.profanity.im"]
                xmlns: http://jabber.org/protocol/caps
                hash: sha-1
                node: http://www.profanity.im
                ver: jra5S1tICxzuMGVnu86Tv5flMBU=


**Nota:Hasta este punto del documento nos hemos referido como *cliente* al primero de los dos que hemos conectado desde la MV al servidor en la ci40**

Los paquetes 180,182,184,186,187,189,190,191,192 y 193 corresponden a la conexión,identificación y vinculación del segundo usuario, y su contenido es análgo a los 108,110,112,114,115,117,118,119,120 y 121 respectivamente, que hemos explicado para el primero. El puerto TCP que asigna la máquina virtual a este segundo cliente (60556) para esta conexión difiere del asignado al primero (60554).

Los paquetes 200 y 201 corresponden a un *Presence Stanza* del cliente2 al servidor y la respuesta de éste.

Los paquetes 288 y 289 corresponden a un *Message Stanza* enviado del cliente1 al cliente2 a través del servidor.

(288) Cliente1 a servidor:

    XMPP Protocol
        MESSAGE [id="prof_msg_2" type="chat"]
            id: prof_msg_2
            to: lucas@ci40.xmpp.sergio.com
            type: chat
            BODY [value="hola que tal estas"]
                value: hola que tal estas
                
(289) Servidor a cliente2:

    XMPP Protocol
        MESSAGE [id="prof_msg_2" type="chat"]
            from: sergio@ci40.xmpp.sergio.com/profanity
            id: prof_msg_2
            to: lucas@ci40.xmpp.sergio.com
            type: chat
            BODY [value="hola que tal estas"]
                value: hola que tal estas
                
Los paquetes 410 y 412, correspondes a un *IQ Stanza* de tipo get desde el cliente1 con en *id* proof_ping_3 y a la rspuesta desde el servidor.

(410) Cliente1 a servidor:

    XMPP Protocol
        IQ [id="prof_ping_3" type="get"]
            id: prof_ping_3
            type: get
            PING [xmlns="urn:xmpp:ping"]
                xmlns: urn:xmpp:ping
        [Response In: 412]

(412) Servidor a cliente1

    XMPP Protocol
        IQ [id="prof_ping_3" type="result" to="sergio@ci40.xmpp.sergio.com/profanity"]
            id: prof_ping_3
            type: result
            to: sergio@ci40.xmpp.sergio.com/profanity
        [Request In: 410]
        
Los paquetes 461-462; 673-674 ;720-721 ;  938-939 ; 974-975 ; 1198-1200 ; 1250-1251 ; 1468-1470 ; 1739-1741 son *IQ Stanzas* con la misma similares.

Los paquetes XMPP en el rango \[943-1386], enumerados a continuación corresponden a envíos al servidor desde alguno de los dos clientes y redifusiones de dicho envío al otro cliente de *Message Stanzas*.

(943) Cliente1 a servidor:

    XMPP Protocol
        MESSAGE [id="prof_msg_6" type="chat"]
            id: prof_msg_6
            to: lucas@ci40.xmpp.sergio.com
            type: chat
            BODY [value="hola"]
                value: hola
                
(944) Servidor a cliente2

    XMPP Protocol
        MESSAGE [id="prof_msg_6" type="chat"]
            from: sergio@ci40.xmpp.sergio.com/profanity
            id: prof_msg_6
            to: lucas@ci40.xmpp.sergio.com
            type: chat
            BODY [value="hola"]
                value: hola

(1042) Cliente 1 a servidor:

    XMPP Protocol
        MESSAGE [id="prof_msg_7" type="chat"]
            id: prof_msg_7
            to: lucas@ci40.xmpp.sergio.com
            type: chat
            BODY [value="estas?"]
                value: estas?

(1044) Servidor a cliente2:

    XMPP Protocol
        MESSAGE [id="prof_msg_7" type="chat"]
            from: sergio@ci40.xmpp.sergio.com/profanity
            id: prof_msg_7
            to: lucas@ci40.xmpp.sergio.com
            type: chat
            BODY [value="estas?"]
                value: estas?
                
(1363) Cliente2 a servidor:

    XMPP Protocol
        MESSAGE [id="prof_msg_6" type="chat"]
            id: prof_msg_6
            to: sergio@ci40.xmpp.sergio.com/profanity
            type: chat
            BODY [value="bien y tu?"]
                value: bien y tu?
            REQUEST [xmlns="urn:xmpp:receipts"] [UNKNOWN]
                xmlns: urn:xmpp:receipts
                [Expert Info (Note/Undecoded): Unknown element: request]
                    [Unknown element: request]
                    [Severity level: Note]
                    [Group: Undecoded]
                    
(1364) Servidor a cliente1:

    XMPP Protocol
        MESSAGE [id="prof_msg_6" type="chat"]
            from: lucas@ci40.xmpp.sergio.com/profanity
            id: prof_msg_6
            to: sergio@ci40.xmpp.sergio.com/profanity
            type: chat
            BODY [value="bien y tu?"]
                value: bien y tu?
            REQUEST [xmlns="urn:xmpp:receipts"] [UNKNOWN]
                xmlns: urn:xmpp:receipts
                [Expert Info (Note/Undecoded): Unknown element: request]
                    [Unknown element: request]
                    [Severity level: Note]
                    [Group: Undecoded]
                    
(1385) Cliente1 a servidor:

    XMPP Protocol
        MESSAGE [id="prof_msg_9" type="chat"]
            id: prof_msg_9
            to: lucas@ci40.xmpp.sergio.com/profanity
            type: chat
            BODY [value="fenomeno"]
                value: fenomeno
                
(1386) Servidor a cliente2:

    XMPP Protocol
        MESSAGE [id="prof_msg_9" type="chat"]
            from: sergio@ci40.xmpp.sergio.com/profanity
            id: prof_msg_9
            to: lucas@ci40.xmpp.sergio.com/profanity
            type: chat
            BODY [value="fenomeno"]
                value: fenomeno

A continuación cerramos el stream desde uno de los clientes para entender como trata el protocolo los mensajes perdidos.

(1713) Cierre de *stream* cliente2 a servidor:

    XMPP Protocol
        STREAM END
        
(1714) Confirmación de cierre servidor a cliente2:

    XMPP Protocol
        STREAM END
        
(1745) Mensaje de cliente1 a servidor para cliente2:

    XMPP Protocol
        MESSAGE [id="prof_msg_13" type="chat"]
            id: prof_msg_13
            to: lucas@ci40.xmpp.sergio.com/profanity
            type: chat
            BODY [value="estas?"]
                value: estas?

Pero este ha cerrado su *stream* por lo que el servidor informa al cliente1 (1746) de que no le puede hacer llegar el mensaje al cliente2:

    XMPP Protocol
        MESSAGE [id="prof_msg_13" type="error"]
            from: lucas@ci40.xmpp.sergio.com/profanity
            id: prof_msg_13
            to: sergio@ci40.xmpp.sergio.com/profanity
            type: error
            ERROR [type="cancel" condition="service-unavailable"]
                type: cancel
                CONDITION: service-unavailable
                [Expert Info (Chat/Response): Stanza error: service-unavailable;]
                    [Stanza error: service-unavailable;]
                    [Severity level: Chat]
                    [Group: Response]

Abrimos de nuevo el *stream* del cliente2 (1821,1823,1825,1827,1828 y 1830). 

(1872) Enviamos un nuevo paquete del cliente1 al servidor:

    XMPP Protocol
        MESSAGE [id="prof_msg_14" type="chat"]
            id: prof_msg_14
            to: lucas@ci40.xmpp.sergio.com
            type: chat
            BODY [value="hola?"]
                value: hola?

(1873) Que de nuevo el servidor reenvía al cliente2:

    XMPP Protocol
        MESSAGE [id="prof_msg_14" type="chat"]
            from: sergio@ci40.xmpp.sergio.com/profanity
            id: prof_msg_14
            to: lucas@ci40.xmpp.sergio.com
            type: chat
            BODY [value="hola?"]
                value: hola?

Luego los paquetes eviados cuando un usuario ha cerrado su *stream* se pierden.
