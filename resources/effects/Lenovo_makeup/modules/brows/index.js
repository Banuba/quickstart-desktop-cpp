'use strict';


const modules_scene_index = require('../scene/index.js');

const Mesh = "modules/brows/Brow.bsm2";

const fragmentShader1 = "modules/brows/brow.frag";

const browImage = "modules/brows/brow.png";

const vertexShader1 = "modules/brows/brow.vert";

const vertexShader2 = "modules/brows/mat_texture.vert";

const fragmentShader2 = "modules/brows/mat_texture.frag";

const MatImages = [
    "modules/brows/mat_texture_BaseColor1.png",
    "modules/brows/mat_texture_BaseColor2.png",
    "modules/brows/mat_texture_BaseColor3.png",
    "modules/brows/mat_texture_BaseColor4.png",
    "modules/brows/mat_texture_BaseColor5.png",
    "modules/brows/mat_texture_BaseColor6.png"
]

class Brows {
    constructor() {
        Object.defineProperty(this, "_face", {
            enumerable: true,
            configurable: true,
            writable: true,
            value: new modules_scene_index.Mesh(new modules_scene_index.FaceGeometry(), [])
        });
        Object.defineProperty(this, "_brows", {
            enumerable: true,
            configurable: true,
            writable: true,
            value: new modules_scene_index.Mesh(new modules_scene_index.Geometry(Mesh), new modules_scene_index.ShaderMaterial({
                vertexShader: vertexShader1,
                fragmentShader: fragmentShader1,
                uniforms: {
                    tex_a: new modules_scene_index.Image(browImage),
                    camera: new modules_scene_index.Scene(),
                    tex_bg_mask: new modules_scene_index.SegmentationMask("BACKGROUND"),
                },
                builtIns: [
                    "bnb_BONES",
                    "bnb_UVMORPH",
                    "bnb_STATICPOS",
                    "bnb_MORPH"
                ],
                state: {
                    blending: "ALPHA",
                    backFaces: false
                },
            }))
        });
        Object.defineProperty(this, "_mat_texture", {
            enumerable: true,
            configurable: true,
            writable: true,
            value: new modules_scene_index.Mesh(new modules_scene_index.Geometry(Mesh), new modules_scene_index.ShaderMaterial({
                vertexShader: vertexShader2,
                fragmentShader: fragmentShader2,
                uniforms: {
                    tex_diffuse: new modules_scene_index.Image(MatImages[0]),
                    tex_bg_mask: new modules_scene_index.SegmentationMask("BACKGROUND"),
                    var_brow_color: new modules_scene_index.Vector4(0, 0, 0, 1),
                },
                builtIns: [
                    "bnb_BONES",
                    "bnb_UVMORPH",
                    "bnb_STATICPOS",
                    "bnb_MORPH"
                ],
                state: {
                    blending: "ALPHA",
                    backFaces: false
                },
            }))
        });
        this._face.add(this._brows, this._mat_texture);
        modules_scene_index.add(this._face, this._brows, this._mat_texture);
        this._brows.visible(false);
        this._mat_texture.visible(false);
    }
    enable() {
        this._brows.visible(false);
        this._mat_texture.visible(true);
    }
    color(color) {
        this._brows.visible(false);
        this._mat_texture.visible(true);
        this._mat_texture.material.uniforms.var_brow_color.value(color);
    }
    disable() {
        this._mat_texture.material.uniforms.var_brow_color.value("0 0 0 1");
        this._brows.visible(false);
        this._mat_texture.visible(false);
    }
    changeBrows(index){
        const num = index < MatImages.length ? index : 0;
        this._mat_texture.material.uniforms.tex_diffuse.load(MatImages[num]);
    }
}

exports.Brows = Brows;
