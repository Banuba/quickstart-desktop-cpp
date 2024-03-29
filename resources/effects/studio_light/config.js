const mat = bnb.scene.getAssetManager().findMaterial("studio_light");

function ambientMultiplier(m)
{
	mat.findParameter("ambient_multiplier").setX(m);
}

// index = 0 or 1
// color = light color, 3 value array in RGB order in [0,1] arnge
// brightness = light radiance multiplier
// pos = light position relative to screen, 2 value array in XY order, [0,0] is lower left, [1,1] is upper right
// R1, R2 = falloff radiuses relative to largest screen side, R1 corresponds to 100% brightness, R2 to 0%
function setLight(index, color, brightness, pos, R1, R2)
{
	if( index == 0 )
	{
		mat.findParameter("light0_radiance").setVector4(new bnb.Vec4(color[0]*brightness, color[1]*brightness, color[2]*brightness, 0));
		mat.findParameter("light0_pos").setVector4(new bnb.Vec4(pos[0], pos[1], R1, R2));
	}
	else if( index == 1 )
	{
		mat.findParameter("light1_radiance").setVector4(new bnb.Vec4(color[0]*brightness, color[1]*brightness, color[2]*brightness, 0));
		mat.findParameter("light1_pos").setVector4(new bnb.Vec4(pos[0], pos[1], R1, R2));
	}
}

ambientMultiplier(0.85);
setLight(0, [0.984, 0.659, 0.290], 1.2, [0.2, 0.5], 0.2, 0.85);
setLight(1, [0.380, 0.437, 1.000], 2.0, [0.85, 0.65], 0.05, 0.65);