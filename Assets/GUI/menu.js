$(document).ready(function() {
  // Main menu
  $("#main-play").click(function() {
    $("#main-menu").css("display", "none");
    $("#lan-menu").css("display", "table");
    $("#lan-refresh").click();
  } );
  $("#main-options").click(function() {
    $("#main-menu").css("display", "none");
    $("#options-menu").css("display", "table");
  } );
  $("#main-exit").click(function() {
    Menu.Exit();
  } );
  // Options menu
  $("#options-player").click(function() {
    alert("Not yet implemented! D:");
  } );
  $("#options-graphics").click(function() {
    alert("Not yet implemented! D:");
  } );
  $("#options-sound").click(function() {
    alert("Not yet implemented! D:");
  } );
  $("#options-keybind").click(function() {
    alert("Not yet implemented! D:");
  } );
  $("#options-back").click(function() {
    $("#options-menu").css("display", "none");
    $("#main-menu").css("display", "table");
  } );
  // Lan menu
  function ClearTable()
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
  $("#lan-connect").click(function() {
    if($("#selected").length < 1)
      return;
    $("#overlay").css("display", "table");
    $("#connecting").css("display", "block");
    var address = $($("#selected").children()[0]).html().split(":");
	if(!address[1] || !address[0])
		return;
    Menu.Connect(address[1], address[0]);
    setTimeout( function() {
        $("#overlay").css("display", "none");
        $("#connecting").css("display", "none");
      },
      3000
    );
  } );
  $("#lan-direct").click(function() {
    $("#overlay").css("display", "table");
    $("#lan-direct-popup").css("display", "block");
  } );
  $("#lan-host").click(function() {
    $("#overlay").css("display", "table");
    $("#lan-host-overlay").css("display", "block");
  } );
  // Lan menu - Direct connect popup
  $("#lan-direct-close").click(function() {
    $("#overlay").css("display", "none");
    $("#lan-direct-popup").css("display", "none");
  } );
  $("#lan-direct-submit").click(function() {
    $("#lan-direct-close").click();
    $("#overlay").css("display", "table");
    $("#connecting").css("display", "block");
    var input = $("#lan-direct-input").val().split(':');
    Menu.Connect(input[1], input[0]);
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
function Hide()
{
  $("body").css("display", "none");
}
function Unide()
{
  $("body").css("display", "block");
}
function AddServer(addr,name,mapfile,players,maxplayers,ping,password)
{
  $("#lan-list").append("<tr><td>"+addr+"</td><td>"+name+"</td><td>"+mapfile+"</td><td>"+players+"</td><td>"+maxplayers+"</td><td>"+ping+"</td><td>"+password+"</td></tr>");
  $("#lan-list").tablesorter();
  $("#lan-list tr").click(function() {
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