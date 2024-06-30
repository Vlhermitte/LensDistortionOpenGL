# File to generate a 3D model from a CSV file with X, Y, Z coordinates
# It is easier to use python scipy and pandas to interpolate and create the 3D model
# But ideally, the 3D model should be created in the game engine itself

import numpy as np
import pandas as pd
from scipy.interpolate import griddata
import sys
import argparse


def create_obj(df, filename='output.obj') -> None:
    # Create MTL file
    mtl_filename = filename.replace('.obj', '.mtl')
    with open(mtl_filename, 'w') as f:
        f.write('# Generated by python script\n')
        f.write('newmtl Material.001\n')
        # Ambient color (RGB 0 to 1)
        f.write('Ka 1.0 1.0 1.0\n')
        # Diffuse color (RGB 0 to 1)
        f.write('Kd 1.0 1.0 1.0\n')
        # Specular color (RGB 0 to 1)
        f.write('Ks 1.0 1.0 1.0\n')
        # Optical density (0 to 10)
        f.write('Ni 1.0\n')
        # Specular exponent (0 to 1000)
        f.write('Ns 1000.0\n')
        # Roughness parameter (0 to 1)
        f.write('Pr 0.0\n')
        # Transparency (0 to 1)
        f.write('d 1.0\n')
        # Illumination model (1 to 10)
        f.write('illum 2\n')  # 2 - diffuse and specular lighting

        # Texture map
        # f.write('map_Kd GroundGrassGreen002_COL_2K.jpg\n')  # Disable texture for now

        # Close file
        f.close()
        print(f'Mtl file created: {mtl_filename} !')

    # Create OBJ file
    with open(filename, 'w') as f:
        f.write('# Generated by python script\n')
        f.write('mtllib ' + mtl_filename.split('/')[-1] + '\n')
        f.write('usemtl Material.001\n')
        f.write('o Material\n')

        # Define thickness
        thickness = 0.5

        # Create original vertices
        for i, row in df.iterrows():
            f.write(f'v {row["x"]} {row["z"]} {row["y"]}\n')  # Y is up-vector for Lens Distortion OpenGL

        # Create offset vertices for thickness
        for i, row in df.iterrows():
            offset_x = row["x"] + thickness * row["nx"]
            offset_y = row["y"] + thickness * row["ny"]
            offset_z = row["z"] + thickness * row["nz"]
            f.write(f'v {offset_x} {offset_z} {offset_y}\n')

        # Create normals
        for i, row in df.iterrows():
            f.write(f'vn {row["nx"]} {row["nz"]} {row["ny"]}\n')

        # Create texture coordinates (UV mapping)
        for i, row in df.iterrows():
            f.write(f'vt {row["x"]} {row["y"]}\n')

        # Create faces for the front and back surfaces
        num_points = int(np.sqrt(len(df)))
        for i in range(num_points - 1):
            for j in range(num_points - 1):
                v1 = i * num_points + j + 1
                v2 = v1 + 1
                v3 = v1 + num_points
                v4 = v3 + 1
                v1_offset = v1 + len(df)
                v2_offset = v2 + len(df)
                v3_offset = v3 + len(df)
                v4_offset = v4 + len(df)

                # Front faces
                f.write(f'f {v1}//{v1} {v2}//{v2} {v4}//{v4}\n')
                f.write(f'f {v1}//{v1} {v4}//{v4} {v3}//{v3}\n')

                # Back faces
                f.write(f'f {v1_offset}//{v1} {v4_offset}//{v4} {v2_offset}//{v2}\n')
                f.write(f'f {v1_offset}//{v1} {v3_offset}//{v3} {v4_offset}//{v4}\n')

                # Side faces
                f.write(f'f {v1}//{v1} {v1_offset}//{v1} {v2_offset}//{v2}\n')
                f.write(f'f {v1}//{v1} {v2_offset}//{v2} {v2}//{v2}\n')

                f.write(f'f {v2}//{v2} {v2_offset}//{v2} {v4_offset}//{v4}\n')
                f.write(f'f {v2}//{v2} {v4_offset}//{v4} {v4}//{v4}\n')

                f.write(f'f {v4}//{v4} {v4_offset}//{v4} {v3_offset}//{v3}\n')
                f.write(f'f {v4}//{v4} {v3_offset}//{v3} {v3}//{v3}\n')

                f.write(f'f {v3}//{v3} {v3_offset}//{v3} {v1_offset}//{v1}\n')
                f.write(f'f {v3}//{v3} {v1_offset}//{v1} {v1}//{v1}\n')

        # Close file
        f.close()
        print(f'Obj file created: {filename} with thickness!')


def interpolate_new_points(df, n_points=200) -> pd.DataFrame:
    n_points_grid = int(np.sqrt(n_points))

    # Interpolate new points
    x = np.linspace(df['x'].min(), df['x'].max(), n_points_grid)
    y = np.linspace(df['y'].min(), df['y'].max(), n_points_grid)
    x, y = np.meshgrid(x, y)

    # Interpolate Z-axis values
    z = griddata((df['x'], df['y']), df['z'], (x, y), method='cubic')

    # Fill NaN values with the neighbors' mean
    mask = np.isnan(z)
    z[mask] = np.interp(np.flatnonzero(mask), np.flatnonzero(~mask), z[~mask])

    # Create a new DataFrame
    df_new = pd.DataFrame({
        'x': x.flatten(),
        'y': y.flatten(),
        'z': z.flatten()
    })

    # Map pointIDString from df to df_new using the index of the points
    df_new['pointIDString'] = df_new.index.map(
        lambda idx: df['pointIDString'][idx] if idx in df.index else f'interpolated_{idx}')

    return df_new


def calculate_normals(df, grid_x, grid_y) -> pd.DataFrame:
    # Calculate normals
    z_reshaped = df['z'].values
    z_reshaped = z_reshaped.reshape(len(grid_y), len(grid_x))
    dz_dx, dz_dy = np.gradient(z_reshaped)

    # Fill NaN values with the neighbors' mean
    mask = np.isnan(dz_dx)
    dz_dx[mask] = np.interp(np.flatnonzero(mask), np.flatnonzero(~mask), dz_dx[~mask])
    mask = np.isnan(dz_dy)
    dz_dy[mask] = np.interp(np.flatnonzero(mask), np.flatnonzero(~mask), dz_dy[~mask])

    normals = np.dstack((-dz_dx, -dz_dy, np.ones_like(dz_dx)))
    norms = np.linalg.norm(normals, axis=2)
    normals /= norms[:, :, np.newaxis]
    normals = normals.reshape(-1, 3)

    df['nx'] = normals[:, 0]
    df['ny'] = normals[:, 1]
    df['nz'] = normals[:, 2]

    return df


def main(*args):
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', nargs='?')
    args = parser.parse_args()

    filename = None
    if args.f:
        filename = args.f
        print(f"Using csv file: {filename}")
    else:
        filename = '../python/terrain.csv'
        print(f"No csv file specified. Using default file: {filename}")

    df = pd.read_csv(filename)
    df['z'] = -df['z']  # Invert Z-axis

    # Interpolate new points
    df_interpolated = interpolate_new_points(df, n_points=300)

    df_with_normals = calculate_normals(
        df_interpolated,
        np.unique(df_interpolated['x']),
        np.unique(df_interpolated['y'])
    )

    output_name = filename.split('/')[-1].split('.')[0]
    output_filename = f'../Resources/Models/Floor/{output_name}.obj'
    create_obj(df_with_normals, filename=output_filename)


if __name__ == '__main__':
    main()
