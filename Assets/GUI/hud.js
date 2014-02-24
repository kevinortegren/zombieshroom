function AddMessage(p_message)
{
	$("#chatlog").html( $("#chatlog").html() + p_message + "<br>");
	$("#chatlog").scrollTop($("#chatlog")[0].scrollHeight);
}
function Send(event)
{
	$('#chatinput').blur();
	HUD.Send($("#chatinput").val());
	$("#chatinput").val("");
	event.preventDefault();
	event.stopPropagation();
}
function SetDeathScreen(p_visible)
{
	$("#deathscreen").css("display", (p_visible?"block":"none"));
}
function SetScoreScreen(p_visible)
{
	$("#scorescreen").css("display", (p_visible?"table":"none"));
}
function UpdateScoreScreen(p_localTeam, p_localName, p_list)
{
	var blue, bluetitle;
	var red, redtitle;
	if(p_localTeam == 2) // Red
	{
		red = $("#TeamScoreTable tbody");
		redtitle = $("#TeamTitle");
		blue = $("#EnemyScoreTable tbody");
		bluetitle = $("#EnemyTitle");
	}
	else // Blue or spectator
	{
		blue = $("#TeamScoreTable");
		bluetitle = $("#TeamTitle");
		red = $("#EnemyScoreTable");
		redtitle = $("#EnemyTitle");
	}
	redtitle.removeClass("blue");
	redtitle.addClass("red");
	redtitle.html("Red");
	bluetitle.removeClass("red");
	bluetitle.addClass("blue");
	bluetitle.html("Blue");
	$("#TeamScoreTable tbody > tr").remove();
	$("#EnemyScoreTable tbody > tr").remove();
	
	for(var player in p_list)
	{
		if(p_list[player][0] == 0 || !p_list[player])
			continue; // Skip spectators
		var team = (p_list[player][0]==1?blue:red);
		team.append("<tr"+(p_list[player][1]==p_localName?" class='highlight'":"")+"><td class='textright'>"+p_list[player][1]+"</td><td class='textcenter'>"+p_list[player][2]+"</td><td class='textcenter'>"+p_list[player][3]+"</td></tr>");
	}
}

$(document).keydown(function(event)
{
	if(!$('#chatinput').is(":focus") && event.keyCode == 13) // Enter
	{
		$('#chatinput').focus();
	}
	else if(event.keyCode == 27) // Esc
	{
		$('#chatinput').blur();
	}
	else if(event.keyCode == 33) // PgUp
	{
		$('#chatlog').scrollTop($('#chatlog').scrollTop() - $('#chatlog').height()/2);
	}
	else if(event.keyCode == 34) // PgDown
	{
		$('#chatlog').scrollTop($('#chatlog').scrollTop() + $('#chatlog').height()/2);
	}
	else
		return true;
});

function OnLoad()
{
	$('#chatinput').blur(function(){ HUD.SetFocus(false); });
	$('#chatinput').focus(function(){ HUD.SetFocus(true); });
	$('#chatinput').keydown(function()
	{
		if(event.keyCode == 13)
			Send(event);
		else
			return true;
	});
}

function Set(p_id, p_value)
{
	var value = p_value;
	if(p_id == "TimeLeft")
	{
		var time = parseFloat(value);
		// If positive, use floor to get minutes, otherwise use ceil (to round down/up towards zero)
		var minutes = time > 0 ? Math.floor(time / 60) : Math.ceil(time / 60);
		var seconds = Math.abs(time - (minutes * 60));
		if(minutes < 10 && minutes >= 0)
			minutes = "0" + minutes;
		if(seconds < 10)
			seconds = "0" + seconds;
		value = "" + minutes + ":" + seconds;
		if(minutes < 2 && minutes >= 0)
			$("#"+p_id).addClass("blink");
		else
			$("#"+p_id).removeClass("blink");
	}
	if(p_id == "Health")
	{
    var oldHealth = parseInt($("#"+p_id).html());
    var newHealth = parseInt(value);
		SetDeathScreen(newHealth <= 0);
    DamageIndicator(oldHealth-newHealth);
    value = newHealth;
	}
	if(p_id == "EndGame")
	{
		SetDeathScreen(p_value == "true");
	}
	if(p_id == "ShowScore")
	{
		SetScoreScreen(p_value == "true");
	}
	if(p_id == "ChargeBarValue")
	{
		if(value == 0)
		{
			$("#chargebar>div").fadeOut(500); // fade out
		}
		else
		{
			$("#chargebar div").stop(); // stop any ongoing animations
			$("#chargebar div").css("display", "block"); // ensure the bar is visible
			//$("#chargebar>div>div").animate({width: ($("#chargebar>div").width()*value)+"px"},2000,"linear",function(){$(this).parent().fadeOut(500);});
			$("#chargebar>div>div").css("width", ($("#chargebar>div").width()*value)+"px"); // resize the charge bar
			// If charge reaches end in either direction, start a fadeout - it'll be aborted if a channel is initiated after charge
		}
	}
	$("#"+p_id).html(value);
}

function SetAbility(p_slot, p_ability)
{
	$("#slot"+p_slot).css("background-image", "url('"+p_ability+".png')");
}

function SetAbilityFocus(p_slot)
{
	$(".slotselected").removeClass("slotselected");
	$("#slot"+p_slot).addClass("slotselected");
}
function StartCooldown(p_slot, p_duration)
{
	var slot = $("#slot"+p_slot+"-cooldown");
	if(parseInt(slot.css("background-position-y").split("px")[0]) == 75)
	{
		slot.css("background-position", "50% 0px");
		slot.animate({'background-position-y': '75px'}, p_duration*1000, 'linear');
	}
}

function DamageIndicator(p_damage)
{
  if(p_damage > 0)
  {
    var opacity = (p_damage/10.0);
    $("#damageTaken").stop().css("opacity", opacity).animate({opacity: 0}, 1000 * opacity);
  }
  // else if(p_damage < 0)
  // {
    // $("#damageTaken").stop().css("opacity", 0);
  // }
}

$(document).ready(function(){
	// SetAbilityFocus(0);
	// SetAbility(1, "TestBall");
	// StartCooldown(1, 3);
	//UpdateScoreScreen(2, "doqunbop", [[2,"doqunbop",1,11],[1,"The Enemy",11,2],[1,"The Enemy2",0,2],[2,"The Ally",3,0]]);
	//Set("ShowScore", "false");
	//UpdateScoreScreen(2,'doqunbop',[[1,'Player',2,2][2,'doqunbop',1,0]]);
  // DamageIndicator(40);
  // setTimeout("DamageIndicator(40);", 2000);
	//Set("ChargeBarValue", 1);
});
