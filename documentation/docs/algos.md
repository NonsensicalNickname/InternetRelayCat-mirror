# Algorithms and Modelling

## Structure Chart

![Structure Chart](assets/structure_chart.drawio.svg)

## Notation
+ Keywords are capitalised.
+ Subroutines in the code are underlined.
+ Subroutines from an external source (i.e. the C++ standard library, FTXUI, or tomlplusplus) are italicised.
+ Subroutine definitions are bold and underlined
+ Notation for indexing uses square brackets, and literal strings are denoted with quotation marks


## Pseudocode 
<pre>
<font size=3>
BEGIN MAINPROGRAM  
    INITIALISATION  
        set handler to <u>handler-constructor</u>   
        set colour-theme to handler.config.theme  
        set screen to <i>interactive-fullscreen-constructor</i>  
        set UI elements to default states  
    END INITIALISATION  
    WHILE running
        <u>render-elements</u>  
        CASEWHERE <i>catch-event</i> is  
            carriage return : <u>send-message</u> (message-content)  
            mousewheel up :   
                IF scroll-amount is not lowest THEN  
                    decrease scroll-amount  
                ENDIF  
            mousewheel down :   
                IF scroll-amount is not highest THEN  
                    increase scroll-amount  
                ENDIF  
            escape : quit program  
        ENDCASE  
        IF <u>poll-for-messages</u> returns true THEN  
            <u>receive-messages</u>  
            <i>refresh-screen</i>  
        ENDIF  
    ENDWHILE
END MAINPROGRAM  
  
BEGIN SUBPROGRAM <h4 style="display: inline"><u>handler-constructor</u></h4>  
    INITIALISATION  
        set config to <u>config-constructor</u>  
        set user to config.users[default-user]  
        set server to config.server[default-server]  
        set server-ip to <i>convert to useable type</i> (server.ip-address)  
        set server-socket to <u>socket-constructor</u> (IPV4, TCP-socket, any, 6667, server-ip)  
        set socket-file-descriptor to server-socket.get-socket-fd  
    END INITIALISATION  
    <i>set socket to non-blocking</i> (socket-file-descriptor)  
    <u>send-message</u> (nick command)  
    <u>send-message</u> (user command)  
END SUBPROGRAM <u>handler-constructor</u>  
  
BEGIN SUBPROGRAM <h4 style="display: inline"><u>config-constructor</u></h4>  
    INITIALISATION  
        IF config file doesn't exist THEN  
            create config file with defaults  
        ENDIF  
        set config-location to <i>get home directory</i>/.config/ircat/config/toml  
    END INITIALISATION  
    set config to <i>parse toml file</i>  
END SUBPROGRAM <u>config-constructor</u>  
  
BEGIN SUBPROGRAM <h4 style="display: inline"><u>socket-constructor</u></h4> (address-family, socket-type,  
                                            protocol, port, ip-address)  
    INITIALISATION  
        set member values to parameters  
    END INITIALISATION  
    IF set socket-file-descriptor to <i>socket</i> (family, socket-type, protocol) returns an error THEN  
        print error-message  
    ENDIF  
    IF socket-purpose is binding THEN  
        <i>set socket option</i> (reuse address, true)  
        <i>bind socket</i>   
    ELSE   
        <i>connect socket</i>  
    ENDIF  
END SUBPROGRAM <u>socket-constructor</u>  
  
BEGIN SUBPROGRAM <h4 style="display: inline"><u>send-message</u></h4> (content)  
    set bytes-sent to 0  
    set total-sent to 0  
    set length to length of content  
    WHILE total-sent < length  
        <i>send</i> (content)  
        set bytes-sent to return value of <i>send</i>  
        set total-sent to total-sent + bytes-sent  
    ENDWHILE  
END SUBPROGRAM <u>send-message</u>  
  
BEGIN SUBPROGRAM <h4 style="display: inline"><u>poll-for-messages</u></h4>  
    set events-count to <i>poll</i> (socket-file-descriptor)  
    IF events-count > 0 THEN  
        append <u>receive-messages</u> to messages  
        return true  
    ENDIF  
END SUBPROGRAM <u>poll-for-messages</u>  
  
BEGIN SUBPROGRAM <h4 style="display: inline"><u>receive-messages</u></h4>  
    set buffer to array of 2048 characters  
    set bytes-received to 0  
    set messages to empty vector of strings  
    set parsed-messages to empty vector of irc-messages   
    <i>recv</i> (socket-file-descriptor, buffer)  
    set bytes-received to return value of <i>recv</i>  
    WHILE there are carriage returns in buffer   
        append a slice of the buffer from the start until the next carriage return to messages  
        remove that slice of the buffer including the carriage return  
    ENDWHILE  
    FOR message IN messages   
        append <u>parse-message</u> (message) to parsed-messsages  
    ENDFOR  
    return parsed-messages  
END SUBPROGRAM <u>receive-messages</u>  
  
BEGIN SUBPROGRAM <h4 style="display: inline"><u>parse-message</u></h4> (message)  
    set trailing to empty string  
    set result to empty irc-message  
    IF message size < 1 THEN  
        return result  
    ENDIF  
    IF message[0] is a ":" THEN  
        set result.prefix to the slice of message between 1 and the first space  
        remove that slice from message  
    ENDIF  
    set result.command to the slice of message from the start until the first space  
    remove that slice from message  
    IF message contains " :" THEN  
        set trailing to the slice of message from after " :" until the end  
        remove that slice from message  
    ENDIF  
    WHILE there are spaces in message  
        append the slice of message from the start until the first space to result.params  
        remove that slice from message  
    ENDWHILE  
    IF there are still characters in message THEN  
        append message to result.params  
    ENDIF  
    append trailinig to result.params  
    return result  
END SUBPROGRAM <u>parse-message</u>   
  
BEGIN SUBPROGRAM <h4 style="display: inline"><u>render-elements</u></h4>  
    set message-box to <i>horizontal box</i> (individual-components)
    set components to <i>vertical box</i> (message-box)
    <i>render</i> (<u>render-messages</u>)
    <i>render</i> (components)
END SUBPROGRAM <u>render-elements</u>  

BEGIN SUBPROGRAM <h4 style="display: inline"><u>render-messages</u></h4>  
    set rendered to vector of elements
    FOR message in messages
        append <i>vertical box</i> (separator-bar, message.prefix, msg.params[last]) to rendered
    ENDFOR
    return rendered
END SUBPROGRAM <u>render-messages</u>  
</font>
</pre>   
  
  
  
  
  
  
  
  
  
  

