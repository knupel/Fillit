#include "../includes/tetro.h"
// try
t_try *new_try() {
  t_try *try;
  try = NULL;
  if(!(try = (t_try*)malloc(sizeof(t_try))))
    return (0);
  ivec2_init(&try->index);
  ivec2_init(&try->offset);
  try->tetro_start = 0;
  try->num = 0;
	try->max = 0;
  return (try);
}


// puzzle
int puzzle_init(t_puzzle **ref_pzl, char blank) {
  (*ref_pzl) = NULL;
  if(!((*ref_pzl) = (t_puzzle*)malloc(sizeof(t_puzzle))))
		return (0);
  (*ref_pzl)->id = 0;
  (*ref_pzl)->line = NULL;
  ivec3_init(&(*ref_pzl)->size);
  ivec2_init(&(*ref_pzl)->start_pos);
  ivec2_init(&(*ref_pzl)->last_pos);
  ivec2_init(&(*ref_pzl)->offset);
  (*ref_pzl)->blank = blank;
  (*ref_pzl)->brick = 0;
  (*ref_pzl)->space = 0;
  (*ref_pzl)->tetro_num = 0;
  (*ref_pzl)->tetro_used = 0;
  (*ref_pzl)->score = 0;
  return(1);
}



// tetro
void tetro_init(t_tetro *t) {
  t->id = 0;
  t->name = 0;
  t->line = NULL;
  ivec2_init(&t->canvas);
  ivec2_init(&t->size);
  ivec2_init(&t->pos);
  ivec2_init(&t->offset);
  ivec2_init(&t->start);
  ivec2_init(&t->end);
  t->pos.set(&t->pos,-1,-1);
  t->start.set(&t->start,-1,-1);
  t->end.set(&t->end,-1,-1);
}

// line
void line_set_arguments(t_line *ln, t_pair *pair) {
  ln->state = 0;
  ln->valid = 0;
  ln->empty = 1;
  ln->offset = 0;
  ln->pair.a = pair->a;
	ln->pair.b = pair->b;
  ln->threshold = 3; // can be used like a failed
  ln->brick = 0;
  ln->col_max = 4;
  ln->space = ln->col_max;
}

int line_init(t_line **ref_line, t_pair *pair) {
  (*ref_line) = NULL;
  if (!((*ref_line) = (t_line*)malloc(sizeof(t_line))))
    return(0);
  line_set_arguments((*ref_line), pair);
  if (!((*ref_line)->content = (char*)malloc(sizeof(char) *((*ref_line)->col_max + 1))))
		return (0);
  return(1);
}

// block
void block_set_arguments(t_block *blk) {
  ivec2_init(&blk->offset);
  blk->valid = 0;
  blk->ready_to_add = 0;
  blk->row = 0;
	blk->brick = 0;
  blk->brick_max = 4;
	blk->row_max = 4;
	blk->col_max = 4;
}

int block_init(t_block **ref_blk) {
  (*ref_blk) = NULL;
  if (!((*ref_blk) = (t_block*)malloc(sizeof(t_block))))
    return(0);
  block_set_arguments((*ref_blk));
  if (!((*ref_blk)->p_line = (t_line*)malloc(sizeof(t_line))))
		return (0);
  if (!((*ref_blk)->p_line->content = (char*)malloc(sizeof(char) *(*ref_blk)->col_max)))
		return (0);
  return(1);
}
