function AddMessage(p_message)
{
	$("#chatlog").html( $("#chatlog").html() + p_message + "<br>");
	$("#chatlog").scrollTop($("#chatlog")[0].scrollHeight);
}
function Send(event)
{
	$('#chatinput').blur();
	ChatSystem.Send($("#chatinput").val());
	$("#chatinput").val("");
	event.preventDefault();
	event.stopPropagation();
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
	else
		return true;
});

function OnLoad()
{
	$('#chatinput').blur(function(){ ChatSystem.SetFocus(false); });
	$('#chatinput').focus(function(){ ChatSystem.SetFocus(true); });
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
	$("#"+p_id).html(p_value);
	if(p_id == "timeleft")
		if(parseInt(p_value.split(':')[0]) < 2)
			$("#"+p_id).class("blink");
		else
			$("#"+p_id).class("");
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
	$("#slot"+p_slot+"-cooldown").css("background-position", "50% 0px");
	$("#slot"+p_slot+"-cooldown").animate({'background-position-y': '75px'}, p_duration*1000, 'linear');
}

// $(document).ready(function(){
	// SetAbilityFocus(0);
	// SetAbility(1, "TestBall");
// });
