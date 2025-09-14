/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 04:15:13 by abelov            #+#    #+#             */
/*   Updated: 2025/09/09 04:25:10 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_sq bsqSolve(t_bsq bsq);
void delete_queue(t_tailhead *head);
int readMap(FILE *stream, t_bsq *bsq, t_tailhead *head);
void fill_bsq(t_bsq *bsq, t_sq result);
int read_map(const char *filename, t_bsq *bsq, t_tailhead *head);


typedef struct s_readbuf
{
	size_t	len;
	ssize_t	nread;
	char	*line;
}	t_readbuf;

/**
 * TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
 * click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{

	t_bsq	bsq = {0x00};

	/* Tail queue head */
	t_tailhead head = TAILQ_HEAD_INITIALIZER(head);

	if (argc != 2)
		exit((fprintf(stderr, "Usage: %s <file>\n", argv[0]), EXIT_FAILURE));
	{
		char *filename = argv[1];

		if (!read_map(filename, &bsq, &head))
		{
			fill_bsq(&bsq, bsqSolve(bsq));
			int i = -1;
			while (++i < bsq.legend.rows)
			{
				fputs(bsq.map[i], stdout);
				fputs("\n", stdout);
			}
			delete_queue(&head);
			free(bsq.map);
		}
	}
	exit(EXIT_SUCCESS);
}

int read_map(const char *filename, t_bsq *bsq, t_tailhead *head)
{
	int		err_code = 0;
	FILE	*stream = fopen(filename, "r");

	if (stream == NULL)
		exit((fprintf(stderr, "fopen: %m"), EXIT_FAILURE));
	int n = fscanf(stream, "%d%c%c%c\n", // NOLINT(*-err34-c)
			   &bsq->legend.rows,
			   &bsq->legend.empty,
			   &bsq->legend.obstacle,
			   &bsq->legend.full);
	if (n != 4) {
		fprintf(stderr, "Failed to parse map header: %m\n");
		exit(1);
	}

	/* Consume the rest of the line (newline or garbage) safely */
//	int c;
//	c = fgetc(stream);
//	while (c != '\n' && c != EOF)
//		c = fgetc(stream);
	err_code = readMap(stream, bsq, head);
	fclose(stream);
	return (err_code);
}

void fill_bsq(t_bsq *bsq, t_sq result)
{
	int i = result.start.y -1;
	while (++i <= result.end.y)
	{
		char *row = bsq->map[i];
		int j = result.start.x - 1;
		while (j < result.end.x)
			row[j++] = bsq->legend.full;
	}
}

extern char *strdup(const char *) __attribute__((noinline));

__attribute__((no_sanitize("all")))
int readMap(FILE *stream, t_bsq *bsq, t_tailhead *head)
{
	t_entry		*n1, *np;
	t_readbuf	buf = {0};
	int			listSize = 0;

	buf.nread = getline(&buf.line, &buf.len, stream);
	while (buf.nread > 0) {
		*strchrnul(buf.line, '\n') = '\0';
		if (bsq->cols == 0)
			bsq->cols = (int)__builtin_strlen(buf.line);
		n1 = malloc(sizeof(struct qentry));
		n1->str = strdup(buf.line);
		TAILQ_INSERT_TAIL(head, n1, entries);
		listSize++;
		if (listSize > bsq->legend.rows)
		{
			free(buf.line);
			delete_queue(head);
			fprintf(stderr, "Error: listSize > bsq->cols\n");
			return (EXIT_FAILURE);
		}
		buf.nread = getline(&buf.line, &buf.len, stream);
	}
	free(buf.line);
	bsq->map = calloc(bsq->legend.rows, sizeof(char *));

	int i = 0;
	TAILQ_FOREACH(np, head, entries) /* Forward traversal */
		bsq->map[i++] = np->str;
	return (0);
}

t_sq bsqSolve(t_bsq bsq)
{
	if (!bsq.map) return  (t_sq){0x00};
	t_sq result = {0x00};

	struct s_legend legend = bsq.legend;

	int (*dp)[bsq.cols + 1] = calloc(2, sizeof(*dp));

	Point  p;
	int maxSideLength = 0;
	p.y = -1;
	while (++p.y < legend.rows)
	{
		size_t curr = p.y & 1;
		size_t prev = curr ^ 1;

		dp[curr][0] = 0;
		p.x = 0;
		while (++p.x < bsq.cols + 1)
		{
			int up = dp[prev][p.x];
			int left = dp[curr][p.x - 1];
			int diag = dp[prev][p.x - 1];

			int minNeighbour = -1;
			if (bsq.map[p.y][p.x - 1] == legend.empty){
				minNeighbour = MIN(diag, MIN(up, left));
				if (minNeighbour + 1 > maxSideLength)
				{
					result.end = p;
					result.start.x = p.x - maxSideLength;
					result.start.y = p.y - maxSideLength;
					maxSideLength = minNeighbour + 1;

				}
			}
			dp[curr][p.x] = minNeighbour + 1;
		}
	}
	free(dp);
	return result;
}

void delete_queue(t_tailhead *head)
{	/* TailQ deletion */
	struct qentry *n1, *n2;

	n1 = TAILQ_FIRST(head);
	while (n1 != NULL) {
		n2 = TAILQ_NEXT(n1, entries);
		free(n1->str);
		free(n1);
		n1 = n2;
	}
	TAILQ_INIT(head);
}
