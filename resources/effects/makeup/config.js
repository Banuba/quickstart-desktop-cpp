'use strict';

const makeup = require('./makeup.js');

Object.assign(globalThis, makeup.m);
/* Feel free to add your custom code below */

function EnlargeEyes(strength){
    FaceMorph.eyes({enlargement: 0.5 * strength});
}

function FaceReshape(strength){
    FaceMorph.face({chin: -0.4 * strength});
    FaceMorph.lips({height: 0.8 * strength});
    FaceMorph.face({jaw_narrowing: 0.35 * strength, chin_narrowing: 0.35 * strength, narrowing: 0.25 * strength, cheekbones_narrowing: -0.5 * strength, forehead: 0.1 * strength});
    FaceMorph.nose({width: 0.3 * strength, length: 0.2 * strength, tip_width: -0.4 * strength});
    FaceMorph.lips({size: 0.3 * strength});
}

function LipsColor(color){
    Lips.color(color);
}

function SkinSmoothing(strength){
   Skin.softening(strength);
}

function BackgroundBlur(strength){
    Background.blur(strength);
}

function BrowsColor(color){
    Brows.color(color);
}

function BrowsImage(num){
    Brows.changeBrows(num)
}

function TeethWhitening(strength){
    Teeth.whitening(strength);
}

function test(){
    EnlargeEyes(1.0);
    FaceReshape(1.0);
    LipsColor("0.898 0.431 0.663 0.9");
    SkinSmoothing(1.0);
    BackgroundBlur(0.55);
    Makeup.blushes("0.871 0.365 0.514 0.5");
    Eyes.color("0.082 0.412 0.780 0.5");
    BrowsColor("0.004 0.004 0.004 0.4");
    BrowsImage(0);
    TeethWhitening(0.5);
    Eyes.whitening(0.2);
    Makeup.eyeshadow("0.322 0.341 0.435 0.5")
}

//BrowsImage(6)
test()
// BrowsColor("0.176 0.102 0.008 1.0");
//BrowsColor("0.424 0.251 0.216 1");
//BrowsColor("0.533 0.361 0.318 1");
//BrowsColor("0.651 0.412 0.337 1.0");
// BrowsColor("0.698 0.580 0.486 0.5");
// test()  