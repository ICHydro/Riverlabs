import glob
import os

Import("env") # import the active SCons environment


def FindInoNodes(env):
    """Return all .ino nodes within a sensor directory.

    PlatformIO uses the software construction tool SCons. In SCons, files are represented
    as 'nodes', which SCons uses to track files involved in the build. This function
    returns the .ino files for a sensor as SCons nodes.

    Args:
        env: The active SCons build environment.

    Returns:
        list: A list of SCons nodes representing the .ino files for a sensor.
    """
    project_root = env.subst("$PROJECT_DIR")

    # custom_sensor_dir is read from platformio.ini for the active env
    sensor_dir = env.GetProjectOption("custom_sensor_dir")
    full_sensor_dir = glob.escape(os.path.join(project_root, sensor_dir))

    # env.Glob() returns SCons nodes for the matching .ino files
    ino_nodes = []
    ino_nodes += env.Glob(os.path.join(full_sensor_dir, "*.ino"))

    return ino_nodes


env.AddMethod(FindInoNodes)
