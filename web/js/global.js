//------------------Variables Globales ---------------------------//
//---------------------------------------------------------------//

var devices_numbers;
var tempera;

//---------------------------------------------------------------//

$("#lock").click(function(){
    var color = $("#lock").css("background-color");

    if (color =='rgb(166, 166, 166)'){
         $("#lock").css("background-color", "#DFDFDF");

         for (i=1; i<=devices_numbers; i++) {
                $("#conf-input"+i).prop("disabled", true);
                $("#conf-time"+i).prop("disabled", true);
            };

         $("#save").hide("slow");
         $("#save2").hide("slow");
         
    }else {
         $("#lock").css("background-color", "#A6A6A6");

         for (i=1; i<=devices_numbers; i++) {
                $("#conf-input"+i).prop("disabled", false);
                $("#conf-time"+i).prop("disabled", false);
           };

         $("#save").show("slow");
         
    }
     
});

$("#save").click(function(){
    var result = confirm("¿Desea guardar los nuevos valores en la base de datos?");
    
    if (result == true){ 

	     for (i=1; i<=devices_numbers; i++) {
	            $.post('php/global.php', {name:"u_ip", disp:i, ip:$('#conf-input'+i).val(), field:"ip"}, function(data){});
	            $.post('php/global.php', {name:"u_move", disp:i, time:$('#conf-time'+i).val(), field:"move"}, function(data){});
	            $("#conf-input"+i).prop("disabled", true);
	            $("#conf-time"+i).prop("disabled", true);
	        };
        
        var data = "update";
        send_server(data,"1");                                                  //Obligo al servidor a que actualize las ip y el tiempo.
        
         $("#lock").css("background-color", "#DFDFDF");
         $("#save").hide("slow");
         $("#save2").hide("slow");
        
    };
    
    });
	


//---------------------------------------------------------------//
//Movimiento de DIV

//Boton de configuraciones

$("#func1").click(function(){
  
//--------------------Config device text with ip number
 
    function assingIp(param, id) {$('#conf-input'+id).val(param);}

    function getDeviceIp(id, callback){
        $.post('php/global.php', {name:"c" , disp:i , field:"ip"}, function(data1){
                callback(data1, id);});}

    for (i=0; i<=devices_numbers; i++) {getDeviceIp(i, assingIp);};


//--------------------Config device text with move time

    function assingMove(param, id) {$('#conf-time'+id).val(param);}

    function getDeviceMove(id, callback){
        $.post('php/global.php', {name:"c" , disp:i , field:"move"}, function(data2){
                callback(data2, id);});}
                                      
    for (i=0; i<=devices_numbers; i++) {getDeviceMove(i, assingMove);}; 


    $("#lock").css("background-color", "")
    $("#bottom").hide("slow");
    $("#botones").hide("slow");
    $("#persiana").hide("slow");
    $("#temporizador").hide("slow");
    
    $("#dispositivos").css("max-height", "380px")
    $("#dispositivos").show("slow");
    $("#save2").hide("slow");
});

//Boton de persianas
$("#func3").click(function(){
    
    
    $("#persiana").css("margin-top", "");
     
    $("#dispositivos").hide("slow");
    $("#botones").show("slow");
    $("#persiana").show("slow");
    $("#temporizador").hide("slow");
    $("#bottom").hide("slow");
    $("#save2").hide("slow");
});
//boton de informacion
$("#func4").click(function(){           
    
    
    $("#dispositivos").hide("slow");
    $("#bottom").slideToggle("slow");
    $("#botones").show("slow");
    
    $("#persiana").css("margin-top", "");
    $("#persiana").show("slow");
    $("#temporizador").hide("slow");
    $("#save2").hide("slow");
});
//boton del timer
$("#func2").click(function(){       
 
    time_screen_reset()   //Funcion para resetear la pantalla de timers
   
    $("#bottom").hide("slow");
    $("#botones").hide("slow");
    $("#dispositivos").hide("slow");
    
    $("#persiana").css("margin-top", "5px");
    $("#persiana").show("slow");
    $("#temporizador").css("height", "315px")
    $("#temporizador").show("slow");
    
});

//Funcion para resetear pantalla de timers

function time_screen_reset(){
    for (z = 1; z <= 6; z++){$("#check_"+z).prop("checked", false);}    //Reseteo todos lo checkbox
    for (z = 1; z <= 6; z++){$("#checkS_"+z).prop("checked", false);}    //Reseteo todos lo checkbox
    for (z = 1; z <= 6; z++){$('#timepicker_s_'+z).val("--:--")}        //reseteo texto area a la espera de persiana
    for (z = 1; z <= 6; z++){$('#timepicker_b_'+z).val("--:--")}        //reseteo texto area a la espera de persiana
    for (z = 1; z <= 6; z++){$("#per"+z).css("background-color", "")}   //reset de color de boton de persianas
    //Reseteo todos los días a negro
    for (z = 1; z <= 3; z++){for ( i = 1; i <= 7; i++) {$("#tempo_show_days_b_"+z+" > #tempo_day"+i).css ("color", "black");}}
    for (z = 1; z <= 3; z++){for ( i = 1; i <= 7; i++) {$("#tempo_show_days_s_"+z+" > #tempo_day"+i).css ("color", "black");}} 
    $("#save2").hide("slow");                                           //reseteo el estado del botón de salvado
}

function save2_check(){
    
    dispo = climb_select() //recupero que dispositivo es el que está seleccionado
        
        if (dispo == 1 ){
                    for(x = 1 ; x <= 6 ; x++){                  //recojo estado de los checkbox
                        if (x <= 3){hour = $('#timepicker_s_'+x).val()}else{
                            y = x-3 ; hour = $('#timepicker_b_'+y).val()}            //recojo la hora que hay en el checkbox
                        
                        if ($('#checkS_'+x).prop("checked") == true){           //recojo estado de checkbos de Short_move
                            short = 1;
                        }else{
                            short = 0;
                        }
                        
                        if ($('#check_'+x).prop("checked") == true){
                            v=1;
                            $.post('php/global.php', {name:"t_checkb", disp:x, data:v, hour:hour, short:short}, function(data){});
                        }else{
                            v=0;
                            $.post('php/global.php', {name:"t_checkb", disp:x, data:v, hour:hour, short:short}, function(data){});  
                        }
                    }
        }
        
        if (dispo == 2 ){
                    
                     
                    for(x = 1 ; x <= 6 ; x++){                  //recojo estado de los checkbox
                       
                        if (x <= 3){hour = $('#timepicker_s_'+x).val()}else{
                            y = x-3 ; hour = $('#timepicker_b_'+y).val()}            //recojo la hora que hay en el checkbox
                        
                        if ($('#checkS_'+x).prop("checked") == true){           //recojo estado de checkbos de Short_move
                            short = 1;
                        }else{
                            short = 0;
                        }
                        
                        if ($('#check_'+x).prop("checked") == true){
                            v=1;
                            $.post('php/global.php', {name:"t_checkb", disp:x+6, data:v, hour:hour, short:short}, function(data){});
                        }else{
                            v=0;
                            $.post('php/global.php', {name:"t_checkb", disp:x+6, data:v, hour:hour, short:short}, function(data){});  
                        }
                    }
        }
        
        if (dispo == 3 ){
                    for(x = 1 ; x <= 6 ; x++){                  //recojo estado de los checkbox
                        if (x <= 3){hour = $('#timepicker_s_'+x).val()}else{
                            y = x-3 ; hour = $('#timepicker_b_'+y).val()}            //recojo la hora que hay en el checkbox
                        
                        if ($('#checkS_'+x).prop("checked") == true){           //recojo estado de checkbos de Short_move
                            short = 1;
                        }else{
                            short = 0;
                        }
                        
                        
                        if ($('#check_'+x).prop("checked") == true){
                            v=1;
                            $.post('php/global.php', {name:"t_checkb", disp:x+12, data:v, hour:hour, short:short}, function(data){});
                        }else{
                            v=0;
                            $.post('php/global.php', {name:"t_checkb", disp:x+12, data:v, hour:hour, short:short}, function(data){});  
                        }
                    }
        }
        
}

function save2_days(){
    
    dispo = climb_select() //recupero que dispositivo es el que está seleccionado
    
    if (dispo == 1 ){inicio = 1;}
    if (dispo == 2 ){inicio = 7;}
    if (dispo == 3 ){inicio = 13;}
    
        id = inicio;
        for (z = 1; z <= 3; z++){
            
                for ( i = 1; i <= 7; i++) {
                    
                    if ($("#tempo_show_days_s_"+z+" > #tempo_day"+i).css ("color") == 'rgb(0, 0, 0)'){
                        v=1;
                        $.post('php/global.php', {name:"t_days", disp:i, data:v, id:id}, function(data){});
                        
                       // alert("es negro")
                    }else{
                        v=0;
                        
                        $.post('php/global.php', {name:"t_days", disp:i, data:v, id:id}, function(data){});
                        //blanco
                    }
                   
                }
                 id=id+1;
            }
        id=inicio+3;
        for (z = 1; z <= 3; z++){
            
                for ( i = 1; i <= 7; i++) {
                    
                    if ($("#tempo_show_days_b_"+z+" > #tempo_day"+i).css ("color") == 'rgb(0, 0, 0)'){
                        v=1;
                        $.post('php/global.php', {name:"t_days", disp:i, data:v, id:id}, function(data){});
                        
                       // alert("es negro")
                    }else{
                        v=0;
                        
                        $.post('php/global.php', {name:"t_days", disp:i, data:v, id:id}, function(data){});
                        //blanco
                    }
                   
                }
                 id=id+1;
            }
}
//Acciones cuando se pulsa el boton
$("#save2").click(function(){
    var result = confirm("¿Desea guardar los nuevos valores en la base de datos?");
    if (result == true){
        save2_check()
        save2_days()
        $("#save2").hide("slow")
        }
})

//Detectar que se ha cambiado algo en la pagina de control de timers
//---------------------------------------------------------------------//

//activo el boton de salvar hora si modifico cualquier hora

for (x=1 ; x <= 3; x++){$("#timepicker_s_"+x).change(function(){$("#save2").show("slow");})}
for (x=1 ; x <= 3; x++){$("#timepicker_b_"+x).change(function(){$("#save2").show("slow");})}

//Activo el boton de salvar hora si pulso sobre checkbox

for (x=1 ; x <= 6; x++){$("#check_"+x).click(function(){$("#save2").show("slow")})}
for (x=1 ; x <= 6; x++){$("#checkS_"+x).click(function(){$("#save2").show("slow")})}
//---------------------------------------------------------------//
//funcion para recuperar el estado de los dias de los temporizadores
function get_days(i,z,t,data){
    
    //i = inicio de cuenta para cortar la cadena
    //z = numero de la capa 1 , 2 ,3 
    //t = si es de subida o bajada "s" or "b"
    //data = variable que contine el array que nos mandda php
    
        for (x=1 ; x <= 7; x++){
            
            if (data.substr(i,1) == "1"){
                
                    $("#tempo_show_days_"+t+"_"+z+" > #tempo_day"+x).css("color", "black");
                }else{
                    $("#tempo_show_days_"+t+"_"+z+" > #tempo_day"+x).css("color", "white");
                }
                 
            i = i +8;       //sumo 8 a la variable para sacar el siguiente dia de la variable data.
                            //console.log(data)
        }
    
}
//Recuperar estado de los temporizadores

    function get_tempo_(id){   
    //Rescatar todos los valores de subida dispositivo 1.
   
    $.post('php/global.php', {name:"p" , disp:id , field:"hour", acc:"s"},function(data){
        
        get_days(34,1,"s",data);                      //sacar datos de la configuración de los dias
        get_days(160,2,"s",data);                      //sacar datos de la configuración de los dias
        get_days(286,3,"s",data);                      //sacar datos de la configuración de los dias
        
        if ((data.substring(103, 104) == 1 )){$('#check_1').prop("checked", "true")}else{$('#check_1').prop("checked","")}
        if ((data.substring(229, 230) == 1 )){$('#check_2').prop("checked", "true")}else{$('#check_2').prop("checked","")}
        if ((data.substring(355, 356) == 1 )){$('#check_3').prop("checked", "true")}else{$('#check_3').prop("checked","")}
        
        if ((data.substring(123, 124) == 1 )){$('#checkS_1').prop("checked", "true")}else{$('#checkS_1').prop("checked","")} 
        if ((data.substring(249, 250) == 1 )){$('#checkS_2').prop("checked", "true")}else{$('#checkS_2').prop("checked","")}
        if ((data.substring(375, 376) == 1 )){$('#checkS_3').prop("checked", "true")}else{$('#checkS_3').prop("checked","")}
        
        $('#timepicker_s_1').val(data.substring(22, 27))              //Extraigo la hora y asigno a textbox
        $('#timepicker_s_2').val(data.substring(148, 153))            //Extraigo la hora y asigno a textbox
        $('#timepicker_s_3').val(data.substring(274, 279))            //Extraigo la hora y asigno a textbox
        })
     
    //Rescatar todos los valores de bajada dispositivo 1
    $.post('php/global.php', {name:"p" , disp:id , field:"hour", acc:"b"},function(data){
        
        get_days(34,1,"b",data);                      //sacar datos de la configuración de los dias
        get_days(160,2,"b",data);                      //sacar datos de la configuración de los dias
        get_days(286,3,"b",data);                      //sacar datos de la configuración de los dias
        
        if ((data.substring(103, 104) == 1 )){$('#check_4').prop("checked", "true")}else{$('#check_4').prop("checked","")}
        if ((data.substring(229, 230) == 1 )){$('#check_5').prop("checked", "true")}else{$('#check_5').prop("checked","")}
        if ((data.substring(355, 356) == 1 )){$('#check_6').prop("checked", "true")}else{$('#check_6').prop("checked","")}
        
        if ((data.substring(123, 124) == 1 )){$('#checkS_4').prop("checked", "true")}else{$('#checkS_4').prop("checked","")} 
        if ((data.substring(249, 250) == 1 )){$('#checkS_5').prop("checked", "true")}else{$('#checkS_5').prop("checked","")}
        if ((data.substring(375, 376) == 1 )){$('#checkS_6').prop("checked", "true")}else{$('#checkS_6').prop("checked","")}
         
        $('#timepicker_b_1').val(data.substring(22, 27))            //Extraigo la hora y asigno a textbox
        $('#timepicker_b_2').val(data.substring(148, 153))            //Extraigo la hora y asigno a textbox
        $('#timepicker_b_3').val(data.substring(274, 279))            //Extraigo la hora y asigno a textbox
        })
    }
 
//---------------------------------------------------------------//
//Acciones de botones de control

$('#boton_up').on('click' , function(){						//Hacer que el boton funcione para enviar a servidor
	
        var data = "Subir";     
	send_server(data,climb_select());               //Llamo a la funcion send_server y le paso el numero de persiana que quiero controlar
})

$('#boton_st').on('click' , function(){						//Hacer que el boton funcione para enviar a servidor
	
        var data = "stop";
        send_server(data,climb_select());               //Llamo a la funcion send_server y le paso el numero de persiana que quiero controlar
})

$('#boton_down').on('click' , function(){						//Hacer que el boton funcione para enviar a servidor
	
        var data = "abajo";
        send_server(data,climb_select());               //Llamo a la funcion send_server y le paso el numero de persiana que quiero controlar
})

//---------------------------------------------------------------//
//Acciones de botones de Heating

$('#bottom1').on('click' , function(){                     //Boton de parada
    
    var data = "hOff";     
    send_server(data,"5");              
})
$('#bottom7').on('click' , function(){                     //Boton de parada
    
    var data = "hStay";     
    send_server(data,"21");              
})
$('#bottom8').on('click' , function(){                     //Boton de parada
    
    var data = "hStay";     
    send_server(data,"21.5");              
})
$('#bottom9').on('click' , function(){                     //Boton de parada
    
    var data = "hStay";     
    send_server(data,"22");              
})
$('#bottom6').on('click' , function(){                     //Boton de parada
    
    var data = "hRaise";     
    send_server(data,"1");              
})


//-----------------------  Funciones -----------------------------//

// Funcion para saber que persiana esta selecciona, 
// Utilizo el color de fondo que he cambiado previamente.

function climb_select(){
		
			    
			    var i;
			    for (i = 1; i <= devices_numbers; i++) {
			        var color = $('#per'+i).css("background-color");
			        if (color == 'rgb(166, 166, 166)'){return i;}
			    }
		   
}

//---------------------------------------------------------------//
// Funcion para traer el estado del dispositivo de la base de datos y actualizar la pagina web //

function automatico(){

        $.post('php/global.php', {name:"b",data:"beat"}, function(data){    //Beat de control de servidor activo
            if (data){  
                $('div#name-data1').text("ONLINE"); //es el servidor
                $("div#name-data1").css("color","#0B0"); //cambio el colo a Verde si está online

	            function modWeb(param, id) {

	                    //console.log(id);
	                    //console.log(param);
	                    
	                    var idplus= id +1;

	                    $("div#name-data"+idplus).text(param);                                          
	            
	                    $("div#funciones").css("background-color",$("#top_down").css('backgroundColor'));

	                    //alert($("#top_down").css('backgroundColor'))

	                    if (param == "online" || param == "ONLINE"){$("div#name-data"+idplus).css("color","#0B0"); }//cambio el colo a Verde si está online
	            
	                    if (param == "offline" || param == "OFFLINE"){$("div#name-data"+idplus).css("color","#E00"); }// cambio el color a Rojo si está offline    

	            }

	            function getDeviceStatus(id, callback){
	                    $.post('php/global.php', {name:"s" , disp:id}, function(data1){
	                    callback(data1, id);});

	             }
                
                for (ki=1; ki<=devices_numbers; ki++) {
                        getDeviceStatus(ki, modWeb);

                 };
                            
             } else {
            		
                    for (ki=1; ki<=devices_numbers+1; ki++) {     
                            $('div#name-data'+ki).text    ("Server off"); 
                            $("div#name-data"+ki).css("color","#E00"); // cambio el color a Rojo si está offline
                            $("div#funciones").css("background-color","#E00"); // cambio el color a Rojo si está offline del fondo de funciones
                        }
            }

    });  
	
	if ($(bottom).is(":visible") == true){
        	send_server("hlocalTemp", "4");
        	send_server("temp", "2");
        	send_server("hstatus", 200);
        	//send_server("hstay", 201);
        	send_server("htrigger", 202);
	}
	
		
}

function wait(ms){
   var start = new Date().getTime();
   var end = start;
   while(end < start + ms) {
     end = new Date().getTime();
  }
}

function get_devices(){

		$.post('php/global.php', {name:"number"},function(data){devices_numbers = data;});

}



function create_device_status_div(){
		
       
        $.post('php/global.php', {name:"number"},function(data){        //extrac max id number from DB, know number of devices
                
                for (var i = 1; i <= data; i++) {
                    var o = i+1;

        
                                    //-------------------------------------------------------Devices status
                                        $('<div/>', {id: 'disp-num',class: 'dispositivo',title: '',text: "Device " +  i + ":",}).appendTo('#disp_status');
                                        $('<div/>', {id: 'name-data' + o,class: 'name-data',title: '',text: "Cargando",}).appendTo('#disp_status');

                                    //-------------------------------------------------------Devices IP

                                        $('<div/>', {id: 'conf-disp',class: 'conf-name',title: '',text: "Device " +  i + ":", }).appendTo('#config');
                                        $('<div/>', {id: 'conf-ip'+i,class: 'conf-ip',title: '',}).appendTo('#config');
                                        $('<input>', {type: 'text',id: 'conf-input'+i,class: 'conf-input',disabled: ""}).appendTo('#conf-ip'+i);
                           


            }

                for (var i = 1; i <= data; i++) {

                    //-------------------------------------------------------Devices Move Time
                          
                    $('<div/>', {id: 'sdiv-conf-time'+i,class: 'conf-ip',title: '',}).appendTo('#config');
                    $('<input>', {type: 'text',id: 'conf-time'+i,class: 'conf-time',disabled: ""}).appendTo('#conf-ip'+i);



                    $.post('php/global.php', {name:"Dhidden" , disp:i},function(data){ //saber si hay que crear el elemento en web

                                        var fields = data.split(",");
                                        var hidden = fields[0];
                                        var i = fields[1];
                                        

                                if (hidden == 0){   //Only create a div if hidden ins't true

                                         $('<div/>', {
                                                id: 'per'+i,
                                                class: 'per',click: function(){
                                                                        
                                                    $('div[id ^= per]').css("background-color", ""); 
                                                   
                                                    $(this).css("background-color", "#A6A6A6");
                                                    
                                                    //Añado todo lo que corresponde a timers
                                                   var call_func = "get_tempo_";
                                                   var currentId = $(this).attr('id');

                                                   if  ($("#temporizador").css("display") == "block"){eval( call_func + '('+ currentId.substr(3)+')' );$("#save2").hide("slow"); }
                                    }
                                    
                    }).appendTo('#persiana');
                                        
                                }

                                })

        
                   
				   
            	}
        })
}  

function create_device_status_div_backup(){
        
       
        $.post('php/global.php', {name:"number"},function(data){        //extrac max id number from DB, know number of devices
                
                for (var i = 1; i <= data; i++) {
                    var o = i+1;

        //-------------------------------------------------------Devices status
                    $('<div/>', {id: 'disp-num',class: 'dispositivo',title: '',text: "Device " +  i + ":",}).appendTo('#disp_status');
                    $('<div/>', {id: 'name-data' + o,class: 'name-data',title: '',text: "Cargando",}).appendTo('#disp_status');

        //-------------------------------------------------------Devices IP

                    $('<div/>', {id: 'conf-disp',class: 'conf-name',title: '',text: "Device " +  i + ":", }).appendTo('#config');
                    $('<div/>', {id: 'conf-ip'+i,class: 'conf-ip',title: '',}).appendTo('#config');
                    $('<input>', {type: 'text',id: 'conf-input'+i,class: 'conf-input',disabled: ""}).appendTo('#conf-ip'+i);
            }

                for (var i = 1; i <= data; i++) {
        //-------------------------------------------------------Devices Move Time
                    //$('<div/>', {id: 'div-conf-time'+i,class: 'conf-name',title: '',text: "Tiempo mov " +  i + " :",}).appendTo('#config');        
                    $('<div/>', {id: 'sdiv-conf-time'+i,class: 'conf-ip',title: '',}).appendTo('#config');
                    //$('<input>', {type: 'text',id: 'conf-time'+i,class: 'conf-time',disabled: ""}).appendTo('#sdiv-conf-time'+i);
                    $('<input>', {type: 'text',id: 'conf-time'+i,class: 'conf-time',disabled: ""}).appendTo('#conf-ip'+i);
                   
                    $('<div/>', {
                                    id: 'per'+i,
                                    class: 'per',click: function(){
                                                            
                                        $('div[id ^= per]').css("background-color", ""); 
                                       
                                        $(this).css("background-color", "#A6A6A6");
                                        
                                        //Añado todo lo que corresponde a timers
                                       var call_func = "get_tempo_";
                                       var currentId = $(this).attr('id');

                                       if  ($("#temporizador").css("display") == "block"){eval( call_func + '('+ currentId.substr(3)+')' );$("#save2").hide("slow"); }

                                    }
                                    
                    }).appendTo('#persiana');
                }
        })
}  
	
//---------------------------------------------------------------//
//Enviar los datos a pagina PHP que se lo envía al servidor.
// DATA es la acción y DATA2 es la perisana que se debe controlar.
function hShowTemp(dato, place){
	
		if (place == "200"){ $('#bottom1').html(dato);} // testigo de encendido de caldera
		if (place == "202"){ $('#bottom3').html(dato);} // trigger de caldera
		if (place == "201"){ $('#bottom2').html("rayse" + dato);}
           

	/*	if (place == "4"){  //Este se usa para el dm35
			dato = dato / 1024 * 330;
			dato = dato -1;
			$('#bottom4').html(dato.toFixed(1) + "º");
		}*/

        if (place == "4"){ //Este se usa para el eth22
            $('#bottom4').html(dato + "º");
        }

		if (place == "2"){ //Este se usa para el eth22
            $('#bottom5').html(dato + "º");
        }

		//if(place == "2"){
		//	dato = dato / 1024 * 330;
		//	dato = dato -1;
		//	$('#bottom5').html(dato.toFixed(1) +"º" );
		//	}
			
		
}

function send_server($data, $data2){
        
             var $valor1 = $data;
             var $valor2 = $data2;
										//recogo el valor del imput para enviar al servidor
	if (($valor1 != '') && ($valor2) )   {                                  //compruebo que mando acción y persiana para evitar problemas
           
			$.post('php/global.php', {name:"t",data:$valor1,data2:$valor2}, function(data){ 	//Llamo a la funcion PHP pasando los valores por post
				
				if (($valor1 == "temp" ) || ($valor1 == "hstatus") || ($valor1 == "hstay") || ($valor2 == "202") || ($valor1 == "hlocalTemp")) {
					hShowTemp(data, $valor2);
					//alert(data);
				}
				
				/*if(data != 1){					//Color rojo en fallo de envio a DEV

					$('div#per'+$data2).css("background-color", "#E00");
					$('#ack').html("Error sending to device: " + $data2 + "valor retornado: " + data); 
			      } else { $('#ack').html("Device: " + $data2 + " - Order: " + $valor1 + " - Answer: " + data );}
				*/
			
		});
	} else {
		
		alert ("Debes seleccionar la persiana a controlar");
	}
};

//---------------------------------------------------------------//
//---------------Temporizador selector de dias--------//
//---------------------------------------------------------------//

// Configurar para que los días sean seleccionables y cambien de color

function createCallback(i , z , t){                         //Funcion para poder cargar la función el los dias de la semana.
           return function(){
                      if ($("#tempo_show_days_"+t+"_"+z+"> #tempo_day"+i).css("color") =='rgb(0, 0, 0)' ){
                           $("#tempo_show_days_"+t+"_"+z+" > #tempo_day"+i).css("color", "white");
                           if($('#save2').css('display') == 'none'){$("#save2").show("slow")}   //Muestro el botón de guardado en cuanto modifico el estado
                      } else {
                          $("#tempo_show_days_"+t+"_"+z+" > #tempo_day"+i).css("color", "black");
                          if($('#save2').css('display') == 'none'){$("#save2").show("slow")}        //Muestro el botón de guardado en cuanto modifico el estado
                         }   
                            }
                        }

for (z = 1; z <= 3; z++){
            for ( i = 1; i <= 7; i++) { 

                         $("#tempo_show_days_b_"+z+" > #tempo_day"+i).click( createCallback( i, z, "b" ) );
                         $("#tempo_show_days_s_"+z+" > #tempo_day"+i).click( createCallback( i, z, "s" ) );
                                                   }
                                               }

//Funcion para saber que tipo de navegador se está utilizando.

function deteccion(){
        return navigator.platform;
}

//---------------------------------------------------------------//
//--------------Cambio de color de modo noche-------------------//

$("#night").click(function(){
    
    if ($('body').css("background-color") == 'rgb(204, 204, 204)'){ 	//NOCHE
            $("body").css("background-color", "#444141");
            $("#top_down").css("background-color", "#515151");
            $("#bottom").css("background-color", "#515151");
            $("#dispositivos").css("background-color", "#515151");
            $("#temporizador").css("background-color", "#515151");
            $("#funciones").css("background-color", "#515151");
            $(".per").css( "background-color", "#B1B1B1" );
            $(".func").css( "background-color", "#B1B1B1" );
            $(".boton_per").css( "background-color", "#B1B1B1" );
            $('#night').css("background-image", "url(IMG/sun.png)")
          
            

            $("#night").css("background-color", "#A6A6A6");   
    }else{																//DIA
            $("body").css("background-color", "#CCCCCC");
            $("#top_down").css("background-color", "#C2C2C2");
            $("#bottom").css("background-color", "#C2C2C2");
            $("#dispositivos").css("background-color", "#C2C2C2");
            $("#temporizador").css("background-color", "#C2C2C2");
            $("#funciones").css("background-color", "#C2C2C2");
            $(".per").css( "background-color", "#DFDFDF" );
            $(".func").css( "background-color", "#DFDFDF" );
            $(".boton_per").css( "background-color", "#DFDFDF" );
            $('#night').css("background-image", "url(IMG/moon.png)")

            $("#night").css("background-color", "#DFDFDF"); 
    }
        
    
});

function comprobarHora(fecha){
  var hora = fecha.getHours();
  console.log(fecha.toLocaleTimeString()
    + (hora>=10 && hora<19 ? ': dia' : ': noche'));

  	if (hora>=10 && hora<19){$('#night').css("background-image", "url(IMG/moon.png)")}
  	else{$("#night").trigger("click"); $('#night').css("background-image", "url(IMG/sun.png)")}
}
var d = new Date();
comprobarHora(d);



