$(document).ready(function() {
  // Splash skip
  $("#splash").click(function() {
    $("#splash").animate({opacity: 0}, 500, "linear", function(){$("#splash").css("display","none");});
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
    $("#settings-menu").css("display", "table");
    LoadSettings();
  } );
  $("#main-exit").click(function() {
    Menu.Exit();
  } );
  // Settings menu
  $("#settings-player").click(function() {
    $("#settings-menu").css("display", "none");
    $("#player-settings-menu").css("display", "table");
  } );
  $("#settings-graphics").click(function() {
    ShowError("Not yet implemented! D:");
  } );
  $("#settings-sound").click(function() {
    ShowError("Not yet implemented! D:");
  } );
  $("#settings-controls").click(function() {
    ShowError("Not yet implemented! D:");
  } );
  $("#settings-back").click(function() {
    $("#settings-menu").css("display", "none");
    $("#main-menu").css("display", "table");
  } );
  // Player settings
  var LoadSettings = function () {
    var settings = Menu.RequestSettings();
    for(var key in settings)
      $("#settings-"+key).val(settings[key]);
  }
  $("#player-settings-back").click(function() {
    $("#player-settings-menu").css("display", "none");
    $("#settings-menu").css("display", "table");
    LoadSettings();
  } );
  $("#player-settings-save").click(function() {
    var settings = {};
    settings["Name"] = $("#settings-Name").val();
    Menu.SaveSettings(settings);
    $("#player-settings-menu").css("display", "none");
    $("#settings-menu").css("display", "table");
  } );
  // Lan menu
  var ClearTable = function ()
  {
    $("#lan-list tr").not($("#lan-list thead tr")).remove();
    $("#lan-list").append("<tr style='display:none;'><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>");
    $("#lan-list").tablesorter();
  };
  $("#lan-back").click(function() {
    $("#main-menu").css("display", "table");
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
    Menu.Connect(address[1], address[0]);
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
    var selectedIndex = $("#lan-host-map")[0].selectedIndex;
    var map = $("#lan-host-map")[0].options[selectedIndex].text;

    $("#lan-host-close").click();
    Menu.Host($("#lan-host-name").val(), 
              $("#lan-host-port").val(), 
              $("#lan-host-password").val(), 
              $("#lan-host-maxplayers").val(), 
              $("#lan-host-matchlength").val() * 60, 
              $("#lan-host-killvictory").val(),
              map
              );
  } );
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
function SetDefaults(name,mapfile,port,password,maxplayers,matchlength,killvictory,maplist)
{
  maplist.forEach(function(map) {
    $("#lan-host-map").append("<option value='"+map+"'>"+map+"</option>");
  });
  $("#lan-host-name").val(name);
  $("#lan-host-map").val(mapfile);
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