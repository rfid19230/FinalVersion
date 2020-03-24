--Cоздание представлений
_________________________________________________________________________

/*
Представление (view), или виртуальная таблица в базе данных Oracle — это 
специфический образ таблицы или набора таблиц, определенный оператором SELECT. 
Представление не существует физически как обычная таблица, являющаяся частью 
табличного пространства.Фактически представление создает виртуальную таблицу 
или подтаблицу только с теми строками и/или столбцами, которые нужно показать 
пользователю.
*/

CREATE OR REPLACE VIEW
	tvview
AS SELECT
	u.u_id as tvv_uid, u.u_f as tvv_uf, u.u_io as tvv_uio, u.u_post as tvv_upost,
	mv.mv_id as tvv_mvid, mv.mv_desc as tvv_mvdesc, mv.mv_closet as tvv_mvcloset,
	tv.tv_take as tvv_take, tv.tv_ret as tvv_ret
FROM
	users u,
	mvalues mv,
	tvalues tv
WHERE
	u.u_id = tv.tv_user
	AND
	mv.mv_id = tv.tv_value
ORDER BY
	tv.tv_take DESC;
DISC;

