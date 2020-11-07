#include "../includes/tetro.h"

void copy_t_puzzle(t_puzzle *dst, t_puzzle *src) {
  dst->id = src->id;
	dst->tetro_num = src->tetro_num;
	dst->tetro_used = src->tetro_used;
	all_lines_dup(&dst->line, src->line);
	reverse_t_line(&dst->line);
	copy_ivec3(&dst->size, &src->size);
	copy_ivec2(&dst->start_pos, &src->start_pos);
	copy_ivec2(&dst->last_pos, &src->last_pos);
	dst->brick = src->brick;
	dst->space = src->space;
}


int size_t_puzzle(t_puzzle *pzl) {
	int size;

	size = 0;
	while(pzl) {
		size++;
		pzl = pzl->next;
	}
	return (size);
}

int init_temp_puzzle_line(t_line **ref_ln, int len) {
  t_line *ln;

  ln = (*ref_ln);
  if (!(ln = (t_line*)malloc(sizeof(t_line))))
    return(0);
  ln->col_max = len;
  if (!(ln->content = (char*)malloc(sizeof(char) *ln->col_max)))
    return (0);
  (*ref_ln) = ln;
  return (1);
}

void set_grid_size(t_puzzle *pzl, t_tetro *tetro, int *inc) {
  if(size_t_tetro(tetro) > 1) {
    while(tetro) {
      pzl->tetro_num++;
      pzl->size.z +=(tetro->size.x + tetro->size.y);
      tetro = tetro->next;
    }
    pzl->size.z += inc[0];
    pzl->size.x = sqrt_ceil(pzl->size.z);
    pzl->size.y = pzl->size.x -1;
  } else {
    pzl->tetro_num = 1;
    pzl->size.x = tetro->size.x;
    pzl->size.y = tetro->size.y;
  }
}

int build_grid_puzzle(t_puzzle **ref_pzl, t_tetro *tetro, int *inc) {
  char c;
  int index;
  t_line *t_line;
  
  set_grid_size(*ref_pzl, tetro, inc);
  c = tetro->line->pair.b;
  fill_t_line(&t_line, c, (*ref_pzl)->size.x);
  t_line->pair.b = c;
  t_line->empty = 1;
  t_line->offset = 0;
  t_line->brick = 0;
  t_line->space = (*ref_pzl)->size.x;

  index = 0;
  while (index < (*ref_pzl)->size.y) {
    add_t_line(&(*ref_pzl)->line, index, t_line);
    // free(t_line);
    index++;
  }
  return (1);
}

void brick_switch(char *line, char target_char, char new_char) {
  int index;

  index = 0;
  while(index < ft_strlen(line)) {
    if(line[index] == target_char) {
      line[index] = new_char;
    }
    index++;
  }
}


void clear_puzzle(t_puzzle **ref_pzl, t_tetro *t) {
  int index;
  char c;
  t_line *ln;

  index = 0;
  while(index < (*ref_pzl)->size.y) {
    c = t->line->pair.b;
    ln = get_t_line((*ref_pzl)->line,index);
    fill_line(&ln->content, c, (*ref_pzl)->size.x);
    index++;
  }
  (*ref_pzl)->brick = 0;
  (*ref_pzl)->space = 0;
}


char get_cell(t_line *src, int x, int y) {
  while(src) {
    if(src->id == y) {
      return (src->content[x]); // dangerous, because there is no out bound exception
    }
    src = src->next;
  }
  return (0);
}

void set_cell(t_line *dst, int x, int y, char c) {
  while(dst) {
    if(dst->id == y) {
      dst->content[x] = c; // dangerous, because there is no out bound exception
      break;
    }
    dst = dst->next;
  }
}

void puzzle_analyze(t_puzzle *pzl) {
  int index_x;
  int index_y;
  char c;

  index_y = 0;
  while(index_y < pzl->size.y) {
    index_x = 0;
    while(index_x < pzl->size.x) {
      c = get_t_line(pzl->line,index_y)->content[index_x];
      if(c == pzl->blank) {
         pzl->space++;
      } else {
        pzl->brick++;
      }
      index_x++;
    }
    index_y++;
  }
}



int line_resolution(t_line *buf_pzl, t_tetro *tetro, t_try *try_piece, int index) {
  int i_x;
  int succes;
  t_line *buf_tetro_ln;

  i_x = 0;
  succes = 0;
  buf_tetro_ln = get_t_line(tetro->line, index);
  while(i_x < tetro->size.x && try_piece->index.x < try_piece->offset.x) {
    if(buf_tetro_ln->content[i_x + tetro->offset.x] == buf_tetro_ln->pair.a) {
      if(buf_pzl->content[i_x + try_piece->index.x] == buf_tetro_ln->pair.b) {
        buf_pzl->content[i_x + try_piece->index.x] = tetro->name;
        succes = 1;
        try_piece->put++;
      } else {
        return (0);
      }
    }
    i_x++;
  }
  return (succes);
}


int complete_line_try(t_line *dst_pzl_ln, t_tetro *tetro, t_try *try_piece) {
  int index_real;
  int index;
  int i_y;
  t_line *buf_pzl;
  t_line *buf_tetro_ln;

  index = tetro->offset.y;
  index_real = 0;
  i_y = 0;
  while(index < tetro->canvas.y) {
    if(index_real > tetro->size.y) {
      return (0);
    }
    buf_tetro_ln = get_t_line(tetro->line, index);
    if(!buf_tetro_ln->empty) {
      if(try_piece->index.y + i_y > try_piece->offset.y + index_real)
        return(0);
      while(try_piece->index.y + i_y <= try_piece->offset.y + index_real) {
        buf_pzl = get_t_line(dst_pzl_ln, try_piece->index.y + i_y);
        if(buf_pzl->space >= buf_tetro_ln->brick) {
          i_y++;
          if(line_resolution(buf_pzl, tetro, try_piece, index)) {
            break;
          } else {
            return(0);
          }
        }
      }
      index_real++;
    }
    index++;
  }
  tetro->pos.copy(&tetro->pos,&try_piece->index);
  return (1);
}


t_puzzle *puzzle_dup(t_puzzle **ref_pzl) {
  t_puzzle *buffer;

  buffer = NULL;
  if(!(buffer = (t_puzzle*)malloc(sizeof(t_puzzle))))
		return (0);
  puzzle_init(buffer, (*ref_pzl)->blank);
  copy_t_puzzle(buffer, (*ref_pzl));
  return (buffer);
}


void update_puzzle(t_puzzle **pzl, t_tetro *tetro) {
  (*pzl)->last_pos.copy(&(*pzl)->last_pos,&tetro->pos);
  if(tetro->id == 0) {
    (*pzl)->start_pos.copy(&(*pzl)->start_pos,&tetro->pos);
  } else {
    if((*pzl)->last_pos.x < (*pzl)->start_pos.x) {
      (*pzl)->start_pos.x = (*pzl)->last_pos.x;
    }
    if((*pzl)->last_pos.y < (*pzl)->start_pos.y) {
      (*pzl)->start_pos.y = (*pzl)->last_pos.y;
    }
  }
  (*pzl)->tetro_used++;
}






int complete_puzzle(t_puzzle **ref_pzl, t_tetro *tetro, t_try *try_piece) {
  t_puzzle *pzl;
  t_tetro *buf_tetro;

  buf_tetro = tetro_dup(&tetro);
  pzl = puzzle_dup(ref_pzl);
  while(try_piece->num < try_piece->max) {
    if(complete_line_try(pzl->line, buf_tetro, try_piece)) {
      update_puzzle(&pzl, buf_tetro);
      break;
    } else {
      update_try(try_piece);
    }
    free_tetro(buf_tetro);
    buf_tetro = tetro_dup(&tetro);
    free_puzzle(pzl);
    pzl = puzzle_dup(ref_pzl);   
  }
  (*ref_pzl) = pzl;
  return (1);
}


int buffering_calc(t_puzzle **ref_pzl, t_tetro *t, t_try *try_piece) {
  t_puzzle *buffer;

  buffer = (*ref_pzl);
  complete_puzzle(&buffer, t, try_piece);
  (*ref_pzl) = buffer;
  return (1);
}


int add_t_puzzle(t_puzzle **dst, t_puzzle *src) {
  t_puzzle *temp_pzl;

  temp_pzl = NULL;
  if(!(temp_pzl = (t_puzzle*)malloc(sizeof(t_puzzle))))
		return (0);
  puzzle_init(temp_pzl, src->blank);
  copy_t_puzzle(temp_pzl, src);
  if((*dst))
    temp_pzl->id = (*dst)->id++;
	temp_pzl->next = (*dst);
	(*dst) = temp_pzl;

  return (1);
}

int puzzle_resolution(t_puzzle **ref_pzl, t_puzzle **ref_pzl_list, t_tetro *tetro, t_try *try_pzl) {
  int index_t;
  int res;
  t_try *try_piece;

  try_piece = new_try();
  index_t = 0;
  res = 1;
  while(index_t < (*ref_pzl)->tetro_num) {
    set_try(try_piece, (*ref_pzl), get_t_tetro(tetro, index_t));
    if(index_t == 0) {
      try_piece->index.copy(&try_piece->index, &try_pzl->index); // why ?
    }
    buffering_calc(ref_pzl, get_t_tetro(tetro, index_t), try_piece);
    index_t++;
  }

  if(try_pzl->num < try_pzl->max) {
    // puzzle_print_info(*ref_pzl);
    if((*ref_pzl)->tetro_used ==  (*ref_pzl)->tetro_num) {
      add_t_puzzle(ref_pzl_list, (*ref_pzl));
    }
    res = 0;
  }
  if(!res) {
    update_try(try_pzl);
    (*ref_pzl)->tetro_used = 0;
    clear_puzzle(ref_pzl, tetro);
    puzzle_resolution(ref_pzl, ref_pzl_list, tetro, try_pzl);
  }
  free(try_piece);
  return(res);
}



int puzzle(t_puzzle **ref_pzl, t_tetro *tetro, t_pair *pair, int *inc) {
  t_puzzle *pzl;
  t_try *try_pzl;

  if(!(pzl = (t_puzzle*)malloc(sizeof(t_puzzle))))
		return (0);
  puzzle_init(pzl, pair->b);
  build_grid_puzzle(&pzl, tetro, inc);
  try_pzl = new_try();
  set_try(try_pzl, pzl, tetro);
  puzzle_resolution(&pzl, ref_pzl, tetro, try_pzl);
  puzzle_analyze(pzl);
  // puzzle_print_info(pzl);
  free_puzzle(pzl);
  free(try_pzl);
  if(!size_t_puzzle((*ref_pzl))) {
    printf("\033[1;31mLA LOOSE J'INCREMENTE\033[0m\n");
    inc[0]++;
    puzzle(ref_pzl, tetro, pair, inc);
    // pas super avec les arrays inc to pass arg :(
  }

  return (size_t_puzzle((*ref_pzl)));
}



