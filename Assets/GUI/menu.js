$(document).ready(function() {
  // Splash skip
  $("#splash").click(function() {
    $("#splash").animate({opacity: 0}, 500, "linear", function(){$("#splash").css("display","none"); JSGlobal.LoadSettings();});
  } );
  setTimeout(function(){$("#splash").click();}, 2000);
  // Main menu
  $("#main-play").click(function() {
    $("#main-menu").css("display", "none");
    $("#lan-menu").css("display", "table");
    $("#lan-refresh").click();
  } );
  $("#main-settings").click(function() {
    $("#main-menu").css("display", "none");
    $("#settings-menu").css("display", "table-row");
  } );
  $("#main-exit").click(function() {
    Menu.Exit();
  } );
  // Lan menu
  var ClearTable = function ()
  {
    $("#lan-list tr").not($("#lan-list thead tr")).remove();
    $("#lan-list").append("<tr style='display:none;'><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>");
    $("#lan-list").tablesorter();
  };
  $("#lan-back").click(function() {
    $("#main-menu").css("display", "table-row");
    $("#lan-menu").css("display", "none");
    $("#selected").attr("id", "");
  } );
  $("#lan-refresh").click(function() {
    ClearTable();
    Menu.Refresh();
  } );
  var PromptPassword = function() {
    $("#overlay").css("display", "table");
    $("#lan-password-popup").css("display", "block");
  };
  var LanConnect = function() {
    if($("#selected").length < 1)
      return;
    var address = $($("#selected").children()[0]).html().split(":");
    if(!address[1] || !address[0])
      return;
    if($($("#selected").children()[0]).html() == "Yes")
    {
      $("#lan-password-address").val($($("#selected").children()[0]).html());
      PromptPassword();
      return;
    }
    Menu.Connect(address[1], address[0], "");
  };
  $("#lan-connect").click( LanConnect );
  $("#lan-list").dblclick( LanConnect );
  
  $("#lan-direct").click(function() {
    $("#overlay").css("display", "table");
    $("#lan-direct-popup").css("display", "block");
  } );
  $("#lan-host").click(function() {
    $("#overlay").css("display", "table");
    $("#lan-host-overlay").css("display", "block");
  } );
  // Error message
  var ErrorClose = function() {
    $("#overlay").css("display", "none");
    $("#error-popup").css("display", "none");
  };
  $("#error-close").click(ErrorClose);
  $("#error-submit").click(ErrorClose);
  // Lan menu - Direct connect popup
  $("#lan-direct-close").click(function() {
    $("#overlay").css("display", "none");
    $("#lan-direct-popup").css("display", "none");
  } );
  $("#lan-direct-submit").click(function() {
    $("#lan-direct-close").click();
    var settings = {};
    settings["lan-direct-address"] = $("#lan-direct-address").val();
    Menu.SaveSettings(settings);
    var input = $("#lan-direct-address").val().split(':');
    var password = $("#lan-direct-password").val();
    Menu.Connect(input[1], input[0], password);
  } );
  // Lan menu - Password prompted
  $("#lan-password-close").click(function() {
    $("#overlay").css("display", "none");
    $("#lan-password-popup").css("display", "none");
  } );
  $("#lan-password-submit").click(function() {
    $("#lan-password-close").click();
    var input = $("#lan-password-address").val().split(':');
    var password = $("#lan-password-password").val();
    Menu.Connect(input[1], input[0], password);
  } );
  // Lan menu - Host popup
  $("#lan-host-close").click(function() {
    $("#overlay").css("display", "none");
    $("#lan-host-overlay").css("display", "none");
  } );
  $("#lan-host-submit").click(function() {
    var selectedMapIndex = $("#lan-host-map")[0].selectedIndex;
    var selectedAbilityIndex = $("#lan-host-abilitypack")[0].selectedIndex;
		
		if(parseFloat($("#lan-host-maxplayers").val()) < parseFloat($("#lan-host-maxplayers").attr("min")))
			$("#lan-host-maxplayers").val($("#lan-host-maxplayers").attr("min"));
		if(parseFloat($("#lan-host-matchlength").val()) < parseFloat($("#lan-host-matchlength").attr("min")))
			$("#lan-host-matchlength").val($("#lan-host-matchlength").attr("min"));
		if(parseFloat($("#lan-host-killvictory").val()) < parseFloat($("#lan-host-killvictory").attr("min")))
			$("#lan-host-killvictory").val($("#lan-host-killvictory").attr("min"));
		
    var settings = {};
    settings["ServerName"] = $("#lan-host-name").val();
    settings["ServerMapfile"] = $("#lan-host-map")[0].options[selectedMapIndex].text;
    settings["ServerAbilityfile"] = $("#lan-host-abilitypack")[0].options[selectedAbilityIndex].text;
    settings["ServerPort"] = $("#lan-host-port").val();
    settings["ServerPassword"] = $("#lan-host-password").val();
    settings["ServerMaxPlayers"] = $("#lan-host-maxplayers").val();
    settings["ServerMatchLength"] = $("#lan-host-matchlength").val();
    settings["ServerKillVictory"] = $("#lan-host-killvictory").val();
    Menu.SaveSettings(settings);
		
    var map = $("#lan-host-map")[0].options[selectedMapIndex].text;
    var ability = $("#lan-host-abilitypack")[0].options[selectedAbilityIndex].text;

    $("#lan-host-close").click();
    Menu.Host($("#lan-host-name").val(), 
              $("#lan-host-port").val(), 
              $("#lan-host-password").val(), 
              $("#lan-host-maxplayers").val(), 
              $("#lan-host-matchlength").val() * 60, 
              $("#lan-host-killvictory").val(),
              map,
              ability
              );
  } );
	//SetDefaults('','','','','','','','',['ColorCube2.1'],[]);
} );

function AddServer(addr,name,mapfile,players,maxplayers,ping,password)
{
  $("#lan-list").append("<tr><td>"+addr+"</td><td>"+name+"</td><td>"+mapfile+"</td><td>"+players+"</td><td>"+maxplayers+"</td><td>"+ping+"</td><td>"+password+"</td></tr>");
  $("#lan-list").tablesorter();
  $("#lan-list tr").not("#lan-list thead tr").click(function() {
    $("#selected").attr("id", "");
    $(this).attr("id", "selected");
  } );
}
function SetDefaults(name,mapfile,abilitypack,port,password,maxplayers,matchlength,killvictory,maplist,abilitylist)
{
  maplist.forEach(function(map) {
    $("#lan-host-map").append("<option value='"+map+"'>"+map+"</option>");
  });
	$("#lan-host-map > option").click(function(){
		$("#lan-host-mapimage > div").css('background-image', "url('../Levels/"+$(this).val()+".gif'),url('../Levels/"+$(this).val()+".jpg'),url('../Levels/"+$(this).val()+".png')");
	});
  abilitylist.forEach(function(ability) {
    $("#lan-host-abilitypack").append("<option value='"+ability+"'>"+ability+"</option>");
  });
  $("#lan-host-name").val(name);
  $("#lan-host-map").val(mapfile);
  $("#lan-host-abilitypack").val(abilitypack);
  $("#lan-host-port").val(port);
  $("#lan-host-password").val(password);
  $("#lan-host-maxplayers").val(maxplayers);
  $("#lan-host-matchlength").val(matchlength);
  $("#lan-host-killvictory").val(killvictory);
}
function ShowError(errorInfo, errorTitle)
{
  $("#error-title").html(errorTitle?errorTitle:"Error");
  $("#error-info").html(errorInfo);
  $("#overlay").css("display", "table");
  $("#error-popup").css("display", "block");
}