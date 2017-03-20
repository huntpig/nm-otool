/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_get_stlist.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulefebvr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/20 10:21:58 by ulefebvr          #+#    #+#             */
/*   Updated: 2017/03/20 10:21:59 by ulefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mach-o/loader.h>
#include <ar.h>

#include <string.h>

#include "nm.h"
#include "misc.h"
#include "swap.h"
#include "libft.h"

t_symtab			*nm_get_stlist(t_ofile *ofile)
{
	uint32_t		magic;
	t_symtab		*stlist;

	stlist = 0;
	magic = *(uint32_t *)ofile->ptr;
	misc_check_swap_need(magic, &(ofile->swap));
	misc_check_filetype(swap_uint32_t(magic, ofile->swap), &(ofile->filetype));
	if (misc_is_macho_file(magic))
	{
		ofile->ncmds = ((struct mach_header *)ofile->ptr)->ncmds;
		stlist = nm_load_macho_command(magic, ofile);
	}
	else if (misc_is_fat(magic))
	{
		stlist = nm_load_fat_command(magic, ofile);
	}
	else if (!ft_strncmp(ofile->ptr, ARMAG, SARMAG))
	{
		stlist = nm_load_archive_command(ofile);
	}
	else
	{
		ft_fdprint(2, "%s\n", "Unrecognized file format");
	}
	return (stlist);
}
