import glob
import os

Import("env")

def FindInoNodes(env):
    """Find all .ino files within the sensor directory for the active environment.

    Args:
        env: The active SCons build environment.

    Returns:
        list: A list of SCons nodes representing the .ino files in the sensor directory.
    """
    project_root = env.subst("$PROJECT_DIR")

    # read from platform.ini for active env
    sensor_dir = env.GetProjectOption("custom_sensor_dir")
    full_sensor_dir = glob.escape(os.path.join(project_root, sensor_dir))

    ino_nodes = []
    ino_nodes += env.Glob(os.path.join(full_sensor_dir, "*.ino"))

    return ino_nodes


env.AddMethod(FindInoNodes)
