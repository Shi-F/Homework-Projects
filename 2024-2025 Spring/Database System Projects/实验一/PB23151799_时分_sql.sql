/*Task 1*/
create schema S_C_T_SC;

create table Student
	(SNO char(11) primary key,
	`NAME` varchar(4),
    GENDER varchar(6),
    AGE int,
    DEPART varchar(3)
    );

create table Teacher
	(TNO char(7) primary key,
	`NAME` varchar(4),
    GENDER varchar(6),
    BIRTHDAY datetime,
    POSITION varchar(25),
    DEPART varchar(3)
    );

create table Course
	(CNO char(8) primary key,
	`NAME` varchar(40),
	CPNO char(8),
	CCREDIT float,
    TNO varchar(7),
	foreign key (CPNO) references Course(CNO),
    foreign key (TNO) references Teacher(TNO)
    );

create table Score
	(SNO char(11),
    CNO char(8),
	DEGREE int,
	SEMESTER char(6),
    primary key(SNO,CNO),
	foreign key (SNO) references Student(SNO),
    foreign key (CNO) references Course(CNO)
    );
    
/*Task 2*/
alter table Student add BIRTHYEAR int;

update Student
set BIRTHYEAR = year(now()) - AGE;

update Student
set AGE = AGE-2;
alter table Student
modify AGE char(2);

alter table Student
drop column BIRTHYEAR;

create table student_course
	(SNO char(11) primary key,
    NUM_COURSE int,
    foreign key(SNO) references Student(SNO)
    );

insert into student_course(SNO, NUM_COURSE)
select student.SNO, count(score.CNO)
from student left outer join score on (student.SNO = score.SNO)
group by student.SNO;

delete from student_course
where NUM_COURSE = 1 or NUM_COURSE = 3;

drop table student_course;

alter table Student
add constraint unique_name unique(`NAME`);
alter table Student
drop constraint unique_name;

alter table Score
drop primary key,
add primary key (SNO,CNO,SEMESTER);

alter table Student
rename column DEPART to DEPARTMENT;

delete from Score
where SNO in
	(select SNO
    from  Student
    where DEPARTMENT = 229);
delete s from Score as s
join (
	select SNO, min(DEGREE) as min_degree
    from Score
    group by (SNO)
) as temp on s.SNO = temp.SNO and s.DEGREE = temp.min_degree;

    
alter table Score
add GRADE char(1);
update Score
set GRADE = (case
when DEGREE >= 90 then 'A'
when DEGREE >= 80 then 'B'
when DEGREE >= 70 then 'C'
when DEGREE >= 60 then 'D'
else 'A'
end);

insert into Student(SNO, `NAME`, GENDER, AGE, DEPARTMENT)
values ('PB231517990', 'SF', 'male', 20, 229);

select SNO, `NAME`
from Student
where DEPARTMENT = 
(select DEPARTMENT
from Student
where SNO = 'PB231517990');

select SNO, `NAME`
from Student
where SNO <> 'PB231517990' AND DEPARTMENT = 
(select DEPARTMENT
from Student
where SNO = 'PB231517990');

select SNO, `NAME`
from Student
where DEPARTMENT <> 
(select DEPARTMENT
from Student
where `NAME` = 'YH');

select SNO, `NAME`
from Student
where DEPARTMENT <> 
(select DEPARTMENT
from Student
where `NAME` = 'YH') AND DEPARTMENT <>
(select DEPARTMENT
from Student
where SNO = 'PB231517990');

select TNO, `NAME`
from Teacher
where DEPART = '229';

select count(*)
from Teacher
where DEPART = '229' or DEPART = '11';

select SNO, `NAME`, AGE
from Student
where AGE = 
(select max(AGE)
from Student);

select SNO, `NAME`, AGE
from Student
where AGE = 
(select min(AGE)
from Student
where DEPARTMENT = 
	(select DEPARTMENT
	from Student
	where SNO = 'PB231517990'));

select Student.SNO, Student.`NAME`, Score.DEGREE
from Student, Score
where Student.SNO = Score.SNO and Score.DEGREE <= 80 and
Score.CNO =
	(select CNO
    from Course
	where `NAME` = 'DB_Design');
select Student.SNO, Student.`NAME`, Score.DEGREE
from Student, Score, Course
where Student.SNO = Score.SNO and Score.CNO = Course.CNO and
Course.`NAME` = 'DB_Design' and Score.DEGREE <= 80;

select SNO, `NAME`
from Student
where SNO in
	(select SNO
    from Score
    where CNO in
		(select CNO
        from Course
        where TNO = 
			(select TNO
            from Teacher
            where `NAME` = 'ZDH')));
            
select distinct Student.SNO, Student.`NAME`
from Student, Course, Teacher, Score
where Student.SNO = Score.SNO and Course.CNO = Score.CNO and Course.TNO = Teacher.TNO and Teacher.`NAME` = 'ZDH';

select Score.SNO, Score.DEGREE
from Score, Course
where Score.CNO = Course.CNO and Course.`NAME` = 'Artificial_Intelligence'
order by Score.DEGREE desc;
select SNO, DEGREE
from Score
where CNO = 
	(select CNO
    from Course
    where `NAME` = 'Artificial_Intelligence')
order by Score.DEGREE desc;

select Course.CNO, Course.`NAME`, avg(Score.DEGREE)
from Course left outer join Score on (Score.CNO = Course.CNO)
group by (Course.CNO);

select Course.CNO, Course.`NAME`, avg(Score.DEGREE)
from Course left outer join Score on (Score.CNO = Course.CNO)
where Course.CCREDIT > 3
group by (Course.CNO);

select SNO
from Student
where not exists
	(select *
    from Course
    where TNO = 'TA90023' and not exists
		(select *
        from Score
        where Score.SNO = Student.SNO and Score.CNO = Course.CNO));
        
select Score.CNO, Course.`NAME`, max(Score.DEGREE), min(Score.DEGREE), max(Score.DEGREE)-min(Score.DEGREE)
from Score, Course
where Score.CNO = Course.CNO
group by (Score.CNO);

select SNO, count(*)
from Score
where DEGREE < 78
group by SNO;

select distinct Teacher.TNO, Teacher.`NAME`
from Teacher, Course
where Teacher.TNO = Course.TNO and Course.CNO in
	(select CNO
    from Score
    where DEGREE < 72);
    
select SNO, `NAME`
from Student
where SNO in
	(select SNO
    from Score
    group by (SNO)
    having count(*) > 6);
    
select SNO
from Student
where not exists
	(select *
    from Score sc1
    where SNO = 'PB230000002' and not exists
		(select *
        from Score sc2
        where sc1.CNO = sc2.CNO and Student.SNO = sc2.SNO));
    
select Student.`NAME`, avg(Score.DEGREE)
from Student left outer join Score on (Student.SNO = Score.SNO)
group by Student.SNO;

select Student.DEPARTMENT, count(Student.SNO), avg(Score.DEGREE)
from Student left join Score on (Student.SNO = Score.SNO)
group by Student.DEPARTMENT;

select `NAME`
from Student
where SNO not in
	(select SNO
    from Score
    where CNO in
		(select CNO
        from Course
        where `NAME` = 'DB_Design' or `NAME` = 'Data_Mining'));
        
select Course.`NAME`, avg_age
from Course left join 
	(select Score.CNO, avg(cast(Student.AGE as signed)) as avg_age
    from Score, Student
    where Score.SNO = Student.SNO
    group by Score.CNO) as temp on (Course.CNO = temp.CNO);

select Student.SNO, Student.`NAME`
from Student, Score
where Score.SNO = Student.SNO and Score.CNO in
	(select CNO
    from Course
    where `NAME` like '%Computer%');

select Score.SNO, Score.CNO, Score.DEGREE
from Score join
	(select CNO, avg(DEGREE) as avg_degree
    from Score
    group by CNO) as course_avg on (Score.CNO = course_avg.CNO)
where Score.DEGREE <= course_avg.avg_degree + 5 and Score.DEGREE >= course_avg.avg_degree - 5;

select Course.CNO, Course.`NAME`, A_count
from Course join
	(select CNO, count(*) as A_count
    from Score
    where GRADE = 'A'
    group by CNO) as Course_acount on (Course.CNO = Course_acount.CNO)
order by A_count desc;

select `NAME`
from Course
where `NAME` like '%\_%r%';

select Teacher.TNO, Course.CNO, avg_degree
from Teacher join 
	(Course join
		(select CNO, avg(DEGREE) as avg_degree
		from Score
		group by (CNO)) as course_avg on (Course.CNO = course_avg.CNO))
	on (Teacher.TNO = Course.TNO)
where (Teacher.TNO, avg_degree) in
	(select Teacher.TNO, max(avg_degree)
    from Teacher join 
		(Course join (select CNO, avg(DEGREE) as avg_degree
		from Score
		group by (CNO)) as course_avg on (Course.CNO = course_avg.CNO))
		on (Teacher.TNO = Course.TNO)
    group by (Teacher.TNO));
    
create index NAME_INDEX
on Student(`NAME`);

create unique index CNO_INDEX
on Course(CNO);

create index SNO_DEGREE_INDEX
on Score(SNO asc, DEGREE desc);

show index from Score;

drop index CNO_INDEX on Course;

create view db_229_student
as
	select *
    from Student
    where DEPARTMENT = 229
with check option;

update db_229_student
set `NAME` = 'SF', SNO = 'PB231517990'
where SNO = 'PB210000031';

select SNO, `NAME`, GENDER
from db_229_student
where AGE < 21;

insert into db_229_student
values('SA242290001','QWE','male','22',229);
select *
from db_229_student;

drop view db_229_student;

create table teacher_salary
	(TNO char(7) primary key,
    SAL float
    );
    
delimiter //
create trigger TS_T1
before update on teacher_salary
for each row
begin
	if new.TNO not in (select TNO from Teacher) then 
		signal sqlstate '10000' set message_text = 'TNO is not in Teacher!' ;
	end if;
end //
create trigger TS_T2
before insert on teacher_salary
for each row
begin
	if new.TNO not in (select TNO from Teacher) then 
		signal sqlstate '10000' set message_text = 'TNO is not in Teacher!' ;
	end if;
end //
delimiter ;

insert into teacher_salary
values('TA88888',1000);

insert into teacher_salary
values('TA90021',30000);
update teacher_salary
set TNO = 'TA88888'
where TNO = 'TA90021';

delimiter //

create trigger TS_T3
before update on teacher_salary
for each row
begin
	if new.TNO in (select TNO from Teacher where POSITION = 'Instructor') then
		if new.SAL < 4000 then
			set new.SAL = 4000;
		elseif new.SAL > 7000 then
			set new.SAL = 7000;
		end if;
	elseif new.TNO in (select TNO from Teacher where POSITION = 'Associate Professor') then
		if new.SAL < 7000 then
			set new.SAL = 7000;
		elseif new.SAL > 10000 then
			set new.SAL = 10000;
		end if;
	else
		if new.SAL < 10000 then
			set new.SAL = 10000;
		elseif new.SAL > 13000 then
			set new.SAL = 13000;
		end if;
	end if;
end //

create trigger TS_T4
before insert on teacher_salary
for each row
begin
	if new.TNO in (select TNO from Teacher where POSITION = 'Instructor') then
		if new.SAL < 4000 then
			set new.SAL = 4000;
		elseif new.SAL > 7000 then
			set new.SAL = 7000;
		end if;
	elseif new.TNO in (select TNO from Teacher where POSITION = 'Associate Professor') then
		if new.SAL < 7000 then
			set new.SAL = 7000;
		elseif new.SAL > 10000 then
			set new.SAL = 10000;
		end if;
	else
		if new.SAL < 10000 then
			set new.SAL = 10000;
		elseif new.SAL > 13000 then
			set new.SAL = 13000;
		end if;
	end if;
end //

delimiter ;

insert into teacher_salary
values('TA90024', 30000);
/*TA90024 is Associate Professor*/

update teacher_salary
set TNO = 'TA90030', SAL = 1
where TNO = 'TA90024';
/*TA90024 is Professor*/

drop trigger TS_T1;
drop trigger TS_T2;
drop trigger TS_T3;
drop trigger TS_T4;

update Score
set DEGREE = NULL
where CNO = 
	(select CNO
    from Course
    where `NAME` = 'DB_Design');
    
select SNO, DEGREE
from Score
where SNO in
	(select SNO
    from Score
    where CNO = 
		(select CNO
		from Course
		where `NAME` = 'DB_Design'))
order by DEGREE asc;

create table Score_new
	(SNO char(11),
    CNO char(8),
	DEGREE int,
	SEMESTER char(6),
    GPA float,
    primary key(SNO,CNO),
	foreign key (SNO) references Student(SNO),
    foreign key (CNO) references Course(CNO)
    );

update Score_new
set GPA = (case
when DEGREE >= 95 then 4.3
when DEGREE >= 90 then 4.0
when DEGREE >= 85 then 3.7
when DEGREE >= 82 then 3.3
when DEGREE >= 78 then 3.0
when DEGREE >= 75 then 2.7
when DEGREE >= 72 then 2.3
when DEGREE >= 68 then 2
when DEGREE >= 65 then 1.7
when DEGREE >= 64 then 1.5
when DEGREE >= 61 then 1.3
when DEGREE >= 60 then 1
else 0
end);

alter table Student
add CREDIT float,
add AVG_SCORE float,
add GPA float;
    
update Student
set CREDIT = 
	(select sum(CCREDIT)
    from Course, Score_new
    where Course.CNO = Score_new.CNO and Score_new.SNO = Student.SNO);
    
update Student
set AVG_SCORE = 
	(select sum(Score_new.DEGREE * Course.CCREDIT)/Student.CREDIT
    from Course, Score_new
    where Course.CNO = Score_new.CNO and Score_new.SNO = Student.SNO);
    
update Student
set GPA = 
	(select sum(Score_new.GPA * Course.CCREDIT)/Student.CREDIT
    from Course, Score_new
    where Course.CNO = Score_new.CNO and Score_new.SNO = Student.SNO);
    
select *
from Student
where (DEPARTMENT, GPA) in
	(select DEPARTMENT, max(GPA)
    from Student
    group by DEPARTMENT);
    