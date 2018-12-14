
const Alexa = require('alexa-sdk');


//=========================================================================================================================================
//TODO: The items below this comment need your attention.
//=========================================================================================================================================

const APP_ID = undefined;

const SKILL_NAME = 'casa';
const GET_FACT_MESSAGE = "Here's your fact: ";
const HELP_MESSAGE = 'You can say tell me a space fact, or, you can say exit... What can I help you with?';
const HELP_REPROMPT = 'What can I help you with?';
const STOP_MESSAGE = 'Goodbye!';

//=========================================================================================================================================
//TODO: Replace this data with your own.  You can find translations of this data at http://github.com/alexa/skill-sample-node-js-fact/data
//=========================================================================================================================================
//const data = ['de momento no tengo datos'];


//=========================================================================================================================================
//Editing anything below this line might break your skill.
//=========================================================================================================================================

var net = require('net');
var HOST = 'echao.asuscomm.com';
var PORT = 6000;

                                            
const handlers = {
    'LaunchRequest': function () {
        this.emit('echao');
    },
    'echao': function () {
        this.response.speak("que pasa tio? ¿bienvenido a Calefaccion!");
        this.emit(':responseReady');
    },
    'subir': function () {
        var numero = this.event.request.intent.slots.grado.value;
           
            var client = new net.Socket();
            client.connect(PORT, HOST, function() {
	        client.write("hRaise," + numero);
            });
            
            client.on('data', function(data) {
                    console.log('DATA: ' + data);
                    // Close the client socket completely
                    client.destroy();
                    });
            client.on('close', function() {
                 console.log('Connection closed');
            });

        this.response.speak("La calefacción subirá la temperatura : " + numero + "grados");
        this.emit(':responseReady');
    },
    'stay': function () {
        var numero = this.event.request.intent.slots.grado.value;
           
            var client = new net.Socket();
            client.connect(PORT, HOST, function() {
	        client.write("hStay," + numero);
            });
            
            client.on('data', function(data) {
                    console.log('DATA: ' + data);
                    // Close the client socket completely
                    client.destroy();
                    });
            client.on('close', function() {
                 console.log('Connection closed');
            });

        this.response.speak("La calefacción mantendrá la temperatura a : " + numero + "grados");
        this.emit(':responseReady');
    },
    'off': function () {
        var numero = 15;
           
            var client = new net.Socket();
            client.connect(PORT, HOST, function() {
	        client.write("hOff," + numero);
            });
            
            client.on('data', function(data) {
                    console.log('DATA: ' + data);
                    // Close the client socket completely
                    client.destroy();
                    });
            client.on('close', function() {
                 console.log('Connection closed');
            });

        this.response.speak("He apagado la calefacción");
        this.emit(':responseReady');
    },
    
    'AMAZON.HelpIntent': function () {
        const speechOutput = "Este es el mensaje de ayuda";
        const reprompt = HELP_REPROMPT;

        this.response.speak(speechOutput).listen(reprompt);
        this.emit(':responseReady');
    },
    'AMAZON.CancelIntent': function () {
        this.response.speak(STOP_MESSAGE);
        this.emit(':responseReady');
    },
    'AMAZON.StopIntent': function () {
        this.response.speak(STOP_MESSAGE);
        this.emit(':responseReady');
    },
};

exports.handler = function (event, context, callback) {
        const alexa = Alexa.handler(event, context, callback);
        alexa.APP_ID = APP_ID;
        alexa.registerHandlers(handlers);
        alexa.execute()
    };




