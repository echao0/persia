<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>Persia</title>

<link rel=”shortcut icon” type=”image/png” href=”/favicon.png”/>
<link href="style.css" rel="stylesheet" type="text/css" />
<meta name="viewport" content="width=device-width"/>

</head>


<body>
<script> document.body.onload = function() {get_devices(); create_device_status_div(); setInterval('automatico()',2000);}</script>


<div id="canvas">

 <div id="titulo">
    	<!-- <div id="titulo1">RM Control</div> -->
     <!--   <div id="temp_max"> Max</div>
        <div id="temp_max_data"> 15</div>
           
        <div id="temp_min_data"> 15</div>
        <div id="temp_min"> Min</div> -->
  </div> 
 
 <div id="top">
	
     
     <div id="temporizador" class="menu_general">
         
         <div id="tempo_titul" > Temporizador Subida </div>
        
         <div id="tempo_show"> 
             <div id="tempo_show_hour_s_1" class="tempo_show_hour">
                    <input id='timepicker_s_1' type='text'name='timepicker' class="tempo_show_hour_2"/>
                    <!-- <img style="width: 10px; height: 10px; margin-top: 5px; float: left; margin-left: 7px;" src="IMG/short.png"  ></img>*/ -->
                    
                    <input id="checkS_1"  class="short_check" type="checkbox"></input>
              </div>          
             
             <div id="tempo_show_days_s_1" class="tempo_show_days"> 
             
                 <div id="tempo_day1" class="tempo_day"> Lu</div>
                 <div id="tempo_day2" class="tempo_day"> Ma</div>
                 <div id="tempo_day3" class="tempo_day"> Mi</div>
                 <div id="tempo_day4" class="tempo_day"> Ju</div>
                 <div id="tempo_day5" class="tempo_day"> Vi</div>
                 <div id="tempo_day6" class="tempo_day"> Sa</div>
                 <div id="tempo_day7" class="tempo_day"> Do</div>
             </div> 
             <div id="tempo_show_check_s_1" class="tempo_check"><input id="check_1" type="checkbox"> </div>
         </div>
     
         <div id="tempo_show"> 
             <div id="tempo_show_hour_s_2" class="tempo_show_hour">
                    <input id='timepicker_s_2' type='text'name='timepicker' class="tempo_show_hour_2"/>
                    <!-- <img style="width: 10px; height: 10px; margin-top: 5px; float: left; margin-left: 7px;" src="IMG/short.png"  ></img>*/ -->
                    <input id="checkS_2"  class="short_check" type="checkbox">
              </div>           
             <div id="tempo_show_days_s_2" class="tempo_show_days">
                 <div id="tempo_day1" class="tempo_day"> Lu</div>
                 <div id="tempo_day2" class="tempo_day"> Ma</div>
                 <div id="tempo_day3" class="tempo_day"> Mi</div>
                 <div id="tempo_day4" class="tempo_day"> Ju</div>
                 <div id="tempo_day5" class="tempo_day"> Vi</div>
                 <div id="tempo_day6" class="tempo_day"> Sa</div>
                 <div id="tempo_day7" class="tempo_day"> Do</div>
             </div> 
             <div id="tempo_show_check_s_2" class="tempo_check"><input id="check_2" type="checkbox"> </div>
         </div>
         
          <div id="tempo_show"> 
              <div id="tempo_show_hour_s_3" class="tempo_show_hour">
                    <input id='timepicker_s_3' type='text'name='timepicker' class="tempo_show_hour_2"/>
                    <!-- <img style="width: 10px; height: 10px; margin-top: 5px; float: left; margin-left: 7px;" src="IMG/short.png"  ></img>*/ -->
                    <input id="checkS_3"  class="short_check" type="checkbox">
              </div>          
              <div id="tempo_show_days_s_3" class="tempo_show_days">
                  <div id="tempo_day1" class="tempo_day"> Lu</div>
                 <div id="tempo_day2" class="tempo_day"> Ma</div>
                 <div id="tempo_day3" class="tempo_day"> Mi</div>
                 <div id="tempo_day4" class="tempo_day"> Ju</div>
                 <div id="tempo_day5" class="tempo_day"> Vi</div>
                 <div id="tempo_day6" class="tempo_day"> Sa</div>
                 <div id="tempo_day7" class="tempo_day"> Do</div>
              </div> 
             <div id="tempo_show_check_s_3" class="tempo_check"><input id="check_3" type="checkbox"> </div>
         </div>
         
         
         <div id="tempo_titul" > Temporizador Bajada </div>
         
        <div id="save2"></div>
         <div id="tempo_show"> 
             <div id="tempo_show_hour_b_1" class="tempo_show_hour">
                    <input id='timepicker_b_1' type='text'name='timepicker' class="tempo_show_hour_2"/>
                    <!-- <img style="width: 10px; height: 10px; margin-top: 5px; float: left; margin-left: 7px;" src="IMG/short.png"  ></img>*/ -->
                    <input id="checkS_4"  class="short_check" type="checkbox">
              </div>          
             <div id="tempo_show_days_b_1" class="tempo_show_days">
                 <div id="tempo_day1" class="tempo_day"> Lu</div>
                 <div id="tempo_day2" class="tempo_day"> Ma</div>
                 <div id="tempo_day3" class="tempo_day"> Mi</div>
                 <div id="tempo_day4" class="tempo_day"> Ju</div>
                 <div id="tempo_day5" class="tempo_day"> Vi</div>
                 <div id="tempo_day6" class="tempo_day"> Sa</div>
                 <div id="tempo_day7" class="tempo_day"> Do</div>
             </div> 
             <div id="tempo_show_check_b_1" class="tempo_check"><input id="check_4" type="checkbox"> </div>
         </div>
     
         <div id="tempo_show"> 
             <div id="tempo_show_hour_b_2" class="tempo_show_hour">
                    <input id='timepicker_b_2' type='text'name='timepicker' class="tempo_show_hour_2"/>
                    <!-- <img style="width: 10px; height: 10px; margin-top: 5px; float: left; margin-left: 7px;" src="IMG/short.png"  ></img>*/ -->
                    <input id="checkS_5"  class="short_check" type="checkbox">
              </div>          
             <div id="tempo_show_days_b_2" class="tempo_show_days">
                 <div id="tempo_day1" class="tempo_day"> Lu</div>
                 <div id="tempo_day2" class="tempo_day"> Ma</div>
                 <div id="tempo_day3" class="tempo_day"> Mi</div>
                 <div id="tempo_day4" class="tempo_day"> Ju</div>
                 <div id="tempo_day5" class="tempo_day"> Vi</div>
                 <div id="tempo_day6" class="tempo_day"> Sa</div>
                 <div id="tempo_day7" class="tempo_day"> Do</div>
             </div> 
             <div id="tempo_show_check_b_2" class="tempo_check"><input id="check_5" type="checkbox"> </div>
         </div>
         
          <div id="tempo_show"> 
              <div id="tempo_show_hour_b_3" class="tempo_show_hour">
                    <input id='timepicker_b_3' type='text'name='timepicker' class="tempo_show_hour_2"/>
                    <!-- <img style="width: 10px; height: 10px; margin-top: 5px; float: left; margin-left: 7px;" src="IMG/short.png"  ></img>*/ -->
                    <input id="checkS_6"  class="short_check" type="checkbox">
              </div>           
              
              <div id="tempo_show_days_b_3" class="tempo_show_days">
                 <div id="tempo_day1" class="tempo_day"> Lu</div>
                 <div id="tempo_day2" class="tempo_day"> Ma</div>
                 <div id="tempo_day3" class="tempo_day"> Mi</div>
                 <div id="tempo_day4" class="tempo_day"> Ju</div>
                 <div id="tempo_day5" class="tempo_day"> Vi</div>
                 <div id="tempo_day6" class="tempo_day"> Sa</div>
                 <div id="tempo_day7" class="tempo_day"> Do</div>
              </div> 
             <div id="tempo_show_check_b_3" class="tempo_check"><input id="check_6" type="checkbox"> </div>
         </div>
     
     </div> 
     
     
		<div id="top_up">
            
            <div id="dispositivos" class=" menu_general">
                <div id="dispositivos_titulo" class=" menu_general_titulo"> Dispositivos </div>
				
				<div id="disp_status">
					<div id="disp-serv" class="dispositivo_top">Server :</div>
					<div id="name-data1" class="name-data">Cargando</div>	
	            </div>                    
                
                <div id="config">
                    <div id="time-title">Time</div>             
                    <div id="conf-disp" class="conf-name">Servidor :</div>
                    <div id="conf-ip0" class="conf-ip"> <input id="conf-input0" type="text" class="conf-input" disabled></input> <input id="conf-time0" class="conf-time" disabled="disabled" type="text"></div>
                
                </div>
                                
                                
                <div id="save"></div>                
                <div id="lock" class="func"></div>
                
               
            </div>
            
       </div>
	   
    	
     
       
     
       <div id="top_down">
       
            <div id="botones" class=" menu_general">
               <!--  <div id="Entretenimiento_titulo" class=" menu_general_titulo"> Entretenimiento </div> -->
                <div id="boton_up" class="boton_per"> </div>
                <div id="boton_st" class="boton_per"> </div>
                <div id="boton_down" class="boton_per"> </div>
            </div>
           <div id="persiana" class="menu_general"> 

               <div id="night" class="funcNigth"></div>
               
           </div>
       
    
            </div>
        
		</div>

	<div id="bottom">  
               <div id="ack"> </div>
          <!--  <a href="http://www.accuweather.com/en/es/barcelona/307297/weather-forecast/307297" class="aw-widget-legal">
            </a><div id="awcc1472638776505" class="aw-widget-current"  data-locationkey="307297" data-unit="c" data-language="es" data-useip="false" data-uid="awcc1472638776505"></div>
            <script type="text/javascript" src="http://oap.accuweather.com/launch.js"></script>
             -->
            
        </div>
	
    <div id="funciones"> 
        
        <div id="func0" class="centrador"> </div>
        <div id="func2" class="func"> </div>
        <div id="func4" class="func"> </div>
        <div id="func3" class="func"> </div>
        <div id="func1" class="func"> </div>
  
    </div>
    
	
</div>
	
</div>

<!-- <script src="https://code.jquery.com/jquery-1.12.2.min.js"> </script> -->
<script src="jquery-3.1.1.min.js"> </script>

<script  src="js/global.js"> </script>    


</body>
</html>
