'use strict';

const makeup = require('./makeup.js');

Object.assign(globalThis, makeup.m);
/* Feel free to add your custom code below */

function EnlargeEyes(strength){
    FaceMesh.eyes(strength)
}

function FaceReshape(strength){
    FaceMesh.face(strength);
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
    EnlargeEyes(2);
    FaceReshape(2);
    LipsColor("1 1 0 1");
    SkinSmoothing(2);
    BackgroundBlur(3);
    Makeup.blushes("1 1 0 1");
    Eyes.color("1 0 0 1");
    BrowsColor("1 0 0 1");
    BrowsImage(3);
    TeethWhitening(1)   
}

//BrowsImage(6)
test()
// BrowsColor("0.176 0.102 0.008 1.0");
//BrowsColor("0.424 0.251 0.216 1");
//BrowsColor("0.533 0.361 0.318 1");
//BrowsColor("0.651 0.412 0.337 1.0");
// BrowsColor("0.698 0.580 0.486 0.5");
// test()  