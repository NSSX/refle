/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tracer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avella <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 17:41:04 by avella            #+#    #+#             */
/*   Updated: 2016/05/27 17:44:05 by avella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_h.h"

void	eye_pos_dir(t_env *e, double x, double y)
{
	double	cw;
	double	ch;

	cw = (e->width - x * 2) / e->height;
	ch = (e->height - y * 2) / e->height;
	e->ray_dir.x = -cos(e->v2) * sin(e->v)
		+ cw * cos(e->v) + ch * sin(e->v2) * sin(e->v);
	e->ray_dir.y = sin(e->v2) + ch * cos(e->v2);
	e->ray_dir.z = cos(e->v2) * cos(e->v)
		+ cw * sin(e->v) - ch * sin(e->v2) * cos(e->v);
	ajust(&(e->ray_dir));
	e->ray_dir.x = -e->ray_dir.x;
	e->ray_dir.z = -e->ray_dir.z;
	e->ray_origin = e->eye_pos;
	e->obj_tmp = NULL;
}

void	give_my_pos(t_env *e)
{
	e->pos.x = e->ray_origin.x + e->value * e->ray_dir.x;
	e->pos.y = e->ray_origin.y + e->value * e->ray_dir.y;
	e->pos.z = e->ray_origin.z + e->value * e->ray_dir.z;
	all_effect(e);
}

t_vec3d  ft_reflex(t_vec3d *incident, t_vec3d *n)
{
  t_vec3d  v;
  
  v.x = incident->x - 2.0 * mult(n, incident) * n->x;
  v.y = incident->y - 2.0 * mult(n, incident) * n->y;
  v.z = incident->z - 2.0 * mult(n, incident) * n->z;
  return (v);
}

t_vec3d	final_color(t_env *e)
{
  t_vec3d normal;
  t_vec3d col_tmp;
  t_vec3d color;
  t_vec3d reflex;

  e->value = 100000000;
  e->obj_tmp = all_inter(e);
  e->color = (t_vec3d){0,0,0};
  color = (t_vec3d){0,0,0};
  if (e->obj_tmp && e->value > 0.0001)
	{
	  /*if(e->obj_tmp->type == 10)
	    {
	      int x = e->x + ((e->v / 0.2) * 50);
	      int y = e->y - ((e->v2 / 0.2) * 50);
	      y -=  e->eye_pos.z * 3;
	      if(x<0)
		x = -x;
	      if(y<0)
		y = -y;
	      int color1 = e->sol->data[y * e->sol->sizeline + x * e->sol->bpp / 8];
	      int color2 = e->sol->data[y * e->sol->sizeline + x * e->sol->bpp / 8 + 1];
	      int color3 = e->sol->data[y * e->sol->sizeline + x * e->sol->bpp / 8 + 2];
	      e->img->data[e->y * e->img->sizeline + e->x * e->img->bpp / 8] = color1;
	      e->img->data[e->y * e->img->sizeline + e->x * e->img->bpp / 8 + 1] = color2;
	      e->img->data[e->y * e->img->sizeline + e->x * e->img->bpp / 8 + 2] = color3;
	      e->no = 1;
	    }
	    else*/
	  e->color = (t_vec3d){e->obj_tmp->color.x, e->obj_tmp->color.y, e->obj_tmp->color.z};
	  if (e->value < 100000000)
	    give_my_pos(e);
	  normal = give_vec(e->obj_tmp, e);
	  double ref;
	  ref = 1;
	  color = e->color;
	  //	  if(e->obj_tmp->type == 1 || e->obj_tmp->type == 0)
	  if(e->obj_tmp->ref > 0)
	    ref = e->obj_tmp->ref;//0.25;
	  if (ref < 1 && e->nb_ref < REF)
	    {
	      e->nb_ref += 1;
	      reflex = ft_reflex(&(e->ray_dir), &normal );
	      ajust(&reflex);
	      normal  =  mult_value(&normal , 0.001);
	      e->pos = plus(&(e->pos), &normal );
	      color =  mult_value(&(color), ref);
	      e->ray_origin = e->pos;
	      e->ray_dir = reflex;
	      col_tmp = final_color(e);
	      col_tmp =  mult_value(&col_tmp, 1 - ref);
	      color = plus(&(color), &col_tmp);
	      }
	}
  /*else
	  {
	    e->no = 1;
	    int x = e->x + ((e->v / 0.2) * 50);
	    int y = e->y + ((e->v2 / 0.2) * 20);
	    if(x<0)
	      x = -x;
	    if(y<0)
	      y = -y;
	    int color1 = e->skybox->data[y * e->skybox->sizeline + x * e->skybox->bpp / 8];
	    int color2 = e->skybox->data[y * e->skybox->sizeline + x * e->skybox->bpp / 8 + 1];
	    int color3 = e->skybox->data[y * e->skybox->sizeline + x * e->skybox->bpp / 8 + 2];
	    e->img->data[e->y * e->img->sizeline + e->x * e->img->bpp / 8] = color1;
	    e->img->data[e->y * e->img->sizeline + e->x * e->img->bpp / 8 + 1] = color2;
	    e->img->data[e->y * e->img->sizeline + e->x * e->img->bpp / 8 + 2] = color3;
	    }*/
	return (color);
}

t_vec3d	give_color(t_env *e, double x, double y)
{
	eye_pos_dir(e, x, y);
	e->color = (t_vec3d){0, 0, 0};
	e->nb_ref = 0;
	e->color = final_color(e);
	return (e->color);
}

void	ray_tracer(t_env *e)
{
	int x;
	int y;
	int a;
	int b;
	int c;
	int d;

	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e->no = 0;
	e->sky = 0;
	e->img = (t_img *)malloc(sizeof(t_img) * 1);
	e->skybox = (t_img *)malloc(sizeof(t_img) * 1);
	e->sol = (t_img *)malloc(sizeof(t_img) * 1);
	e->img->img_ptr = mlx_new_image(e->mlx, e->width, e->height);
	e->img->data = mlx_get_data_addr(e->img->img_ptr,
					 &e->img->bpp, &e->img->sizeline, &e->img->endian);
	e->skybox->img_ptr = mlx_xpm_file_to_image(e->mlx, "cart.xpm", &(c), &(d));
	e->skybox->data = mlx_get_data_addr(e->skybox->img_ptr,
					    &(e->skybox->bpp), &(e->skybox->sizeline), &(e->skybox->endian));
	e->sol->img_ptr = mlx_xpm_file_to_image(e->mlx, "sol.xpm", &(a), &(b));
	e->sol->data = mlx_get_data_addr(e->sol->img_ptr,
                                            &(e->sol->bpp), &(e->sol->sizeline), &(e->sol->endian));
	y = 0;
	while (y < e->height)
	{
	  e->y = y;
		x = 0;
		while (x < e->width)
		{
		  e->s = 0;
		  e->x = x;
		  e->color = give_color(e, x, y);
		  if(e->no == 0)
		    pixel_put_to_img(e, x, y);
		  e->no = 0;
		  x++;
		}
		y++;
	}
	mlx_put_image_to_window(e->mlx, e->win, e->img->img_ptr, 0, 0);
}
