import bpy
import json
import os
import math
import mathutils

PROJECT_DIR = '/Users/jake/Projects/random_jon_stuff/jon_and_jake/tinyraytracer/tinyraytracer'
PROGRAM_PATH = os.path.join(PROJECT_DIR, 'tinyraytracer')

def calcFOV(camera):
    sensor_width = camera.data.sensor_width
    focal_length = camera.data.lens

    return 2 * math.atan(sensor_width / (2 * focal_length))



def createObjRotation(obj):
    return {
        'x' : obj.rotation_euler.x,
        'y' : obj.rotation_euler.z,
        'z' : -obj.rotation_euler.y
    }

def createObjPosition(obj):
    return {
            'x' : obj.location.x,
            'y' : obj.location.z,
            'z' : -obj.location.y
        }

def readObject(obj):
    if obj.type == 'CAMERA':
        raise Exception('should not be passing in camera')

    elif obj.type == 'CURVE':
        return readCurve(obj)

    elif obj.type == 'MESH':
        return readMesh(obj)

    elif obj.type == 'EMPTY':
        return readEmpty(obj)
    elif obj.type == 'LIGHT':
        return None #we don't treat lights as regular objects
    else:
        raise NotImplementedError()

def readEmpty(obj):
    return {
        "type": "empty",
        "imageFileName": "",
        'center' : createObjPosition(obj),
        'radius' : -1,
        'rotation' : createObjRotation(obj),
        "velocity" : {
            "x": 0,
            "y": 0,
            "z": 0
        },
        "angularVelocity": {
            "x": 0,
            "y": 0,
            "z": 0
        },
        'children' : [readObject(child) for child in obj.children if readObject(child) is not None]
    }

def readCurve(obj):
    radius = round(obj.dimensions.x / 2, 2)
    innerRadius = radius * .600
    return {
        "type": "ring",
        "imageFileName": f"./assets/{obj.name}_diffuse.png",
        'center' : createObjPosition(obj),
        'radius' : radius,
        'innerRadius' : innerRadius,
        'rotation' : createObjRotation(obj),
        "velocity" : {
            "x": 0,
            "y": 0,
            "z": 0
        },
        "angularVelocity": {
            "x": 0,
            "y": 0,
            "z": 0
        },
        'children' : [readObject(child) for child in obj.children if readObject(child) is not None]
    }
    
def readLight(obj):

    #light may be parented to something, so we want to decompose all of that if that is the case
    if obj.parent is not None:
        loc, rot, scale = obj.matrix_world.decompose()
        obj.location = loc
    lightLocation = createObjPosition(obj)
    lightRotation = createObjRotation(obj)

    return lightLocation, lightRotation

def readCamera(obj):
    #desiredRot = mathutils.Matrix.Rotation(-math.pi / 2, 3, 'X')
    #rotation = obj.rotation_euler.to_matrix() @ desiredRot
    
    euler_angles = obj.rotation_euler
    
    cameraLocation = createObjPosition(obj)
    cameraRotation = {
        'x' : euler_angles.x,
        'y' : euler_angles.z,
        'z' : -euler_angles.y
    }

    fov = calcFOV(obj)

    return cameraLocation, cameraRotation, fov

def readMesh(obj):
    if "Sphere" in obj.name or "Jupiter" in obj.name:
        filename = f"./assets/jupiter_diffuse.png"
    else:
        filename = f"./assets/{obj.name}_diffuse.png"
    return {
        "type": "sphere",
        "imageFileName": filename,
        'center' : createObjPosition(obj),
        'radius' : obj.data.vertices[0].co.magnitude,
        'rotation' : createObjRotation(obj),
        "velocity" : {
            "x": 0,
            "y": 0,
            "z": 0
        },
        "angularVelocity": {
            "x": 0,
            "y": 0,
            "z": 0
        },
        'children' : [readObject(child) for child in obj.children if readObject(child) is not None]
    }


def render_frame(frame_num):
    bpy.context.scene.frame_set(frame_num)
    objects = []
    lights = []
    for obj in bpy.data.objects:
        if obj.type == 'CAMERA':
            cameraLocation, cameraRotation, fov = readCamera(obj)
            
        elif obj.type == 'LIGHT':
            lightLocation, lightRotation = readLight(obj)
            lights.append({
                'location' : lightLocation,
                'rotation' : lightRotation,
                'intensity' : 0.9,
            })
            
        else:

            if obj.parent is None:
                objects.append(readObject(obj))
            
    scene = {
    "frameSize": {
        "width": bpy.data.scenes[0].render.resolution_x,
        "height": bpy.data.scenes[0].render.resolution_y
    },
    "cameraLocation" : cameraLocation,
    "cameraRotation" : cameraRotation,
    "fov" : fov,
    "objects" : objects,
    "lights" : lights
    }
    json_string = json.dumps(scene, indent=1)
    outfile = os.path.join(PROJECT_DIR, 'assets', 'scripts', f'frame_{frame_num}.json')
    with open(outfile, 'w') as f:
        f.write(json_string)   

    command = f"{PROGRAM_PATH} -scenefile {outfile} &"
    print("running: " + command)
    os.system(command)


os.chdir(PROJECT_DIR)
for frame in range(bpy.context.scene.frame_start, bpy.context.scene.frame_end + 1):
    render_frame(frame)
    