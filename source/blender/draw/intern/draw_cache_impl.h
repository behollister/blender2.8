/*
 * Copyright 2016, Blender Foundation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Contributor(s): Blender Institute
 *
 */

/** \file draw_cache_impl.h
 *  \ingroup draw
 */

#ifndef __DRAW_CACHE_IMPL_H__
#define __DRAW_CACHE_IMPL_H__

struct Gwn_Batch;
struct GPUMaterial;
struct ListBase;
struct CurveCache;
struct ParticleSystem;
struct ModifierData;

struct Curve;
struct Lattice;
struct Mesh;

/* Expose via BKE callbacks */
void DRW_curve_batch_cache_dirty(struct Curve *cu, int mode);
void DRW_curve_batch_cache_free(struct Curve *cu);

void DRW_mesh_batch_cache_dirty(struct Mesh *me, int mode);
void DRW_mesh_batch_cache_free(struct Mesh *me);

void DRW_lattice_batch_cache_dirty(struct Lattice *lt, int mode);
void DRW_lattice_batch_cache_free(struct Lattice *lt);

void DRW_particle_batch_cache_dirty(struct ParticleSystem *psys, int mode);
void DRW_particle_batch_cache_free(struct ParticleSystem *psys);

/* Curve */
struct Gwn_Batch *DRW_curve_batch_cache_get_wire_edge(struct Curve *cu, struct CurveCache *ob_curve_cache);
struct Gwn_Batch *DRW_curve_batch_cache_get_normal_edge(
        struct Curve *cu, struct CurveCache *ob_curve_cache, float normal_size);
struct Gwn_Batch *DRW_curve_batch_cache_get_overlay_edges(struct Curve *cu);
struct Gwn_Batch *DRW_curve_batch_cache_get_overlay_verts(struct Curve *cu);

struct Gwn_Batch *DRW_curve_batch_cache_get_triangles_with_normals(struct Curve *cu, struct CurveCache *ob_curve_cache);

/* Curve (Font) */
struct Gwn_Batch *DRW_curve_batch_cache_get_overlay_cursor(struct Curve *cu);
struct Gwn_Batch *DRW_curve_batch_cache_get_overlay_select(struct Curve *cu);

/* DispList */
struct Gwn_Batch *BLI_displist_batch_calc_surface(struct ListBase *lb);

/* Lattice */
struct Gwn_Batch *DRW_lattice_batch_cache_get_all_edges(struct Lattice *lt, bool use_weight, const int actdef);
struct Gwn_Batch *DRW_lattice_batch_cache_get_all_verts(struct Lattice *lt);
struct Gwn_Batch *DRW_lattice_batch_cache_get_overlay_verts(struct Lattice *lt);

/* Mesh */

struct Gwn_Batch **DRW_mesh_batch_cache_get_surface_shaded(
        struct Mesh *me, struct GPUMaterial **gpumat_array, uint gpumat_array_len);
struct Gwn_Batch **DRW_mesh_batch_cache_get_surface_texpaint(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_surface_texpaint_single(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_weight_overlay_edges(struct Mesh *me, bool use_wire, bool use_sel);
struct Gwn_Batch *DRW_mesh_batch_cache_get_weight_overlay_faces(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_weight_overlay_verts(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_all_edges(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_all_triangles(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_triangles_with_normals(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_triangles_with_normals_and_weights(struct Mesh *me, int defgroup);
struct Gwn_Batch *DRW_mesh_batch_cache_get_triangles_with_normals_and_vert_colors(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_triangles_with_select_id(struct Mesh *me, bool use_hide);
struct Gwn_Batch *DRW_mesh_batch_cache_get_points_with_normals(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_all_verts(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_fancy_edges(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_overlay_triangles(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_overlay_triangles_nor(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_overlay_loose_edges(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_overlay_loose_edges_nor(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_overlay_loose_verts(struct Mesh *me);
struct Gwn_Batch *DRW_mesh_batch_cache_get_overlay_facedots(struct Mesh *me);

void DRW_mesh_cache_sculpt_coords_ensure(struct Mesh *me);

/* Particles */
struct Gwn_Batch *DRW_particles_batch_cache_get_hair(struct ParticleSystem *psys, struct ModifierData *md);
struct Gwn_Batch *DRW_particles_batch_cache_get_dots(struct ParticleSystem *psys);

#endif /* __DRAW_CACHE_IMPL_H__ */
