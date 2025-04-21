# 数据库实验一之SQL练习 实验报告

时分 PB23151799

## MySQL环境的安装配置

安装配置流程如下：

在官网下载MySQL最新版本（8.0.41-winx64）；

配置环境变量；

写入配置文件（my.ini）；

在CMD中完成MySQL环境初始化；

安装MySQL Workbench。

安装和配置结果如下：

![image-20250419114425123](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250419114425123.png)

![image-20250419114722223](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250419114722223.png)

![image-20250419114851350](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250419114851350.png)

![image-20250419114606797](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250419114606797.png)

## SQL练习

### 任务一

1. 创建模式：

   ```mysql
   create schema S_C_T_SC;
   ```

2. 创建表Student：

   ```mysql
   create table Student
   	(SNO char(11) primary key,
   	`NAME` varchar(4),		/*NAME是保留字*/
       GENDER varchar(6),
       AGE int,
       DEPART varchar(3)
       );
   ```

   ![屏幕截图 2025-04-15 164509](C:\Users\时分\Pictures\Screenshots\屏幕截图 2025-04-15 164509.png)

3. 创建表Teacher：

   ```mysql
   create table Teacher
   	(TNO char(7) primary key,
   	`NAME` varchar(4),
       GENDER varchar(6),
       BIRTHDAY datetime,
       POSITION varchar(25),
       DEPART varchar(3)
       );
   ```

   ![image-20250415165431377](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415165431377.png)

4. 创建表Course：

   ```mysql
   create table Course
   	(CNO char(8) primary key,
   	`NAME` varchar(40),
   	CPNO char(8),
   	CCREDIT float,
       TNO varchar(7),
       foreign key (CPNO) references Course(CNO),
       foreign key (TNO) references Teacher(TNO)
       );
   ```

   ![image-20250415171055149](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415171055149.png)

5. 创建表Score：

   ```mysql
   create table Score
   	(SNO char(11),
       CNO char(8),
   	DEGREE int,
   	SEMESTER char(6),
       primary key(SNO,CNO),
   	foreign key (SNO) references Student(SNO),
       foreign key (CNO) references Course(CNO)
       );
   ```

   ![image-20250415171559650](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415171559650.png)

### 任务二

#### 修改基本表

1. 在学⽣表中增加⼀个新的属性列BIRTHYEAR（出⽣年份可为INT类型）  

   ```mysql
   alter table Student add BIRTHYEAR int;
   ```

   ![image-20250415171842523](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415171842523.png)

2. 计算每个学⽣的出⽣年份（使⽤当前年份减去年龄）  

   ```mysql
   update Student
   set BIRTHYEAR = year(now()) - AGE;
   ```

   ![image-20250415172458666](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415172458666.png)

3. 将每个学⽣的年龄减去2，并将年龄的类型从int改成char 

   ```mysql
   update Student
   set AGE = AGE-2;
   alter table Student
   modify AGE char(2);
   ```

   ![image-20250415172657149](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415172657149.png)

   ![image-20250415173106008](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415173106008.png)

4. 将BIRTHYEAR列删除 

   ```mysql
   alter table Student
   drop column BIRTHYEAR;
   ```

   ![image-20250415173257922](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415173257922.png)

5. 创建⼀个学⽣选课课程数量表： student_course(SNO,NUM_COURSE)，两个属性分别表⽰授课学⽣学号，选课课程数量， 其中 SNO 是主键。

   ```mysql
   create table student_course
   	(SNO char(11) primary key,
       NUM_COURSE int,
       foreign key(SNO) references Student(SNO)
       );
   ```

   ![image-20250415173823476](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415173823476.png)

6. ⽤⼀条语句，结合表 score 记录，为表 student 中所有学⽣，在表 student_course 添加对应选课数量记录（⽤到查询，⽽不是⼿动数 NUM 插⼊）

   ```mysql
   insert into student_course(SNO, NUM_COURSE)
   select Student.SNO, count(Score.CNO)
   from Student left outer join Score on (Student.SNO = Score.SNO)	/*使用左连接保证包括Student中所有学生*/
   group by Student.SNO;
   ```

   ![image-20250415175005898](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415175005898.png)

7. 删除student_course中选修1门或者3门课的学⽣，然后删除student_course表 

   ```mysql
   delete from student_course
   where NUM_COURSE = 1 or NUM_COURSE = 3;
   ```

   ![image-20250415175444787](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415175444787.png)

   ```mysql
   drop table student_course;
   ```

   ![image-20250415175623300](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415175623300.png)

8. 在Student表中添加学⽣姓名必须取唯⼀值的约束条件，然后再将这个约束条件删除 

   ```mysql
   alter table Student
   add constraint unique_name unique(`NAME`);
   ```

   ![image-20250417143307676](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417143307676.png)

   ```mysql
   alter table Student
   drop constraint unique_name;
   ```

   ![image-20250417143354804](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417143354804.png)
   
9. 删除Score表的复合主键，然后建⽴新的主键（SNO,CNO,SEMESTER）

   ```mysql
   alter table Score
   drop primary key,
   add primary key (SNO,CNO,SEMESTER);
   ```

   ![image-20250415192153868](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415192153868.png)

10. 修改Student表中DEPART列的列名为DEPARTMENT  

    ```mysql
    alter table Student
    rename column DEPART to DEPARTMENT;
    ```

    ![image-20250415192336330](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415192336330.png)

11. 在score表中删除DEPARTMENT为229的学⽣的课程记录，然后在score表中为每个学⽣删除其最低分的课程记录

    ```mysql
    delete from Score
    where SNO in
    	(select SNO
        from  Student
        where DEPARTMENT = 229);
    ```

    ![image-20250415192707534](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415192707534.png)

    ```mysql
    /*MySQL不允许删除正在查询的表*/
    /*需要利用查找得到临时表并内连接*/
    delete s from Score as s
    join (
    	select SNO, min(DEGREE) as min_degree
        from Score
        group by (SNO)
    ) as temp on s.SNO = temp.SNO and s.DEGREE = temp.min_degree;
    ```

    ![image-20250415232123065](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415232123065.png)

12. 在成绩表（Score）中增加⼀个新的属性列GRADE（等级，CHAR类型），并根据成绩（Score）填充等级：90以上为'A'，80-89为'B'，70-79为'C'，60-69为'D'，60以下为'F'。提示，可以使⽤CASE WHEN语句

    ```mysql
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
    ```

    ![image-20250415232149818](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415232149818.png)

13. 在Student表中插⼊你⾃⼰的个⼈数据（包含你的学号，姓名缩写，性别，年龄，系号），注意：为了防⽌学号撞车我们将学号扩充了⼀位，所以你在插⼊的时候也可以将⾃⼰的学号扩充⼀位（⽐如加⼀个0）

    ```mysql
    insert into Student(SNO, `NAME`, GENDER, AGE, DEPARTMENT)
    values ('PB231517990', 'SF', 'male', '20', 229);
    ```

    ![屏幕截图 2025-04-15 193729](C:\Users\时分\Pictures\Screenshots\屏幕截图 2025-04-15 193729.png)

#### 查询

1. 查询和你属于同⼀个系的学⽣学号和姓名(包括你本⼈)

   ```mysql
   select SNO, `NAME`
   from Student
   where DEPARTMENT = 
   	(select DEPARTMENT
   	from Student
   	where SNO = 'PB231517990');
   ```

   ![image-20250415213145675](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415213145675.png)

2. 查询和你属于同⼀个系的学⽣学号和姓名(不包括你本人）

   ```mysql
   select SNO, `NAME`
   from Student
   where SNO <> 'PB231517990' AND DEPARTMENT = 
   	(select DEPARTMENT
   	from Student
   	where SNO = 'PB231517990');
   ```

   ![image-20250415213306585](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415213306585.png)

3. 查询和YH同学不属于同⼀个系的学⽣学号和姓名。 

   ```mysql
   select SNO, `NAME`
   from Student
   where DEPARTMENT <> 
   	(select DEPARTMENT
   	from Student
   	where `NAME` = 'YH');
   ```

![image-20250415213435836](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415213435836.png)

4. 查询和你以及YH同学都不在⼀个系的学⽣学号和姓名

   ```mysql
   select SNO, `NAME`
   from Student
   where DEPARTMENT <> 
   	(select DEPARTMENT
   	from Student
   	where `NAME` = 'YH') AND DEPARTMENT <>
   		(select DEPARTMENT
   		from Student
   		where SNO = 'PB231517990');
   ```

   ![image-20250415213631069](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415213631069.png)

5. 查询229系⽼师的⼯号和姓名。

   ```mysql
   select TNO, `NAME`
   from Teacher
   where DEPART = '229';
   ```

   ![image-20250415213744479](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415213744479.png)

6. 查询 11 系和 229 系教师的总人数。 

   ```mysql
   select count(*)
   from Teacher
   where DEPART = '229' or DEPART = '11';
   ```

   ![image-20250415213857215](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415213857215.png)

7. 查询年龄最⼤的学⽣的学号、姓名和年龄。  

   ```mysql
   select SNO, `NAME`, AGE
   from Student
   where AGE = 
   	(select max(AGE)
   	from Student);	/*MySQL在执行MAX等函数时会自动进行类型转换（字符到数值）*/
   ```

   ![image-20250415214533405](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415214533405.png)

8. 查询你的系中年龄最⼩的学⽣的学号、姓名和年龄 

   ```mysql
   select SNO, `NAME`, AGE
   from Student
   where AGE = 
   	(select min(AGE)
   	from Student
   	where DEPARTMENT = 
   		(select DEPARTMENT
   		from Student
   		where SNO = 'PB231517990'));
   ```

   ![image-20250415214902475](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415214902475.png)

9. 查询选修 DB_Design 课程(不能通过课程号直接进⾏查询)且成绩在 80 分及以下的学⽣的学号、姓名和分数

   ```mysql
   /*解法一：使用嵌套查询*/
   select Student.SNO, Student.`NAME`, Score.DEGREE
   from Student, Score
   where Student.SNO = Score.SNO and Score.DEGREE <= 80 and
   Score.CNO =
   	(select CNO
       from Course
   	where `NAME` = 'DB_Design');
   
   /*解法二：使用连接查询*/
   select Student.SNO, Student.`NAME`, Score.DEGREE
   from Student, Score, Course
   where Student.SNO = Score.SNO and Score.CNO = Course.CNO and
   Course.`NAME` = 'DB_Design' and Score.DEGREE <= 80;
   ```

   ![image-20250415232416581](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415232416581.png)

10. 查询选修过“ZDH”⽼师课程(不能通过课程号直接进⾏查询)的学⽣学号和姓名（去掉重复⾏）。  

    ```mysql
    /*解法一：使用嵌套查询*/
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
                
    /*解法二：使用连接查询*/
    select distinct Student.SNO, Student.`NAME`
    from Student, Course, Teacher, Score
    where Student.SNO = Score.SNO and 
    Course.CNO = Score.CNO and 
    Course.TNO = Teacher.TNO and 
    Teacher.`NAME` = 'ZDH';
    
    /*也可以结合嵌套和连接查询*/
    ```

    ![image-20250415232537128](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415232537128.png)

11. 查询选过某课程的学⽣学号和分数，并按分数降序展⽰。（某课程是指 course 表中的某⼀课程名 NAME，你⾃⾏选择；分数指的是选的这门课的成绩，不包括选这门课的同学的其他成绩）。

    ```mysql
    /*解法一、使用连接查询*/
    select Score.SNO, Score.DEGREE
    from Score, Course
    where Score.CNO = Course.CNO and Course.`NAME` = 'Artificial_Intelligence'
    order by Score.DEGREE desc;
    
    /*解法二、使用嵌套查询*/
    select SNO, DEGREE
    from Score
    where CNO = 
    	(select CNO
        from Course
        where `NAME` = 'Artificial_Intelligence')
    order by Score.DEGREE desc;
    ```

    ![image-20250415232554272](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415232554272.png)

12. 查询每门课的平均成绩，其中每⾏包含课程号、课程名和平均成绩（包括平均成绩为NULL，即该课没有成绩）。

    ```mysql
    select Course.CNO, Course.`NAME`, avg(Score.DEGREE)
    from Course left outer join Score on (Score.CNO = Course.CNO)	/*要求包含所有课程，使用外连接*/
    group by (Course.CNO);
    ```

    ![image-20250415233114371](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415233114371.png)

13. 查询学分⼤于 3(不包括3) 的课程的平均成绩，其中每⾏包含课程号、课程名和平均成绩（包括平均成绩为NULL，即该课没有成绩）

    ```mysql
    select Course.CNO, Course.`NAME`, avg(Score.DEGREE)
    from Course left outer join Score on (Score.CNO = Course.CNO)
    where Course.CCREDIT > 3
    group by (Course.CNO);
    ```

    ![image-20250415233100000](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415233100000.png)

14. 查询⾄少选修了⼯号TNO=”TA90023”的⽼师（ZDH ⽼师）开设的所有课程的学⽣学号

    ```mysql
    select SNO
    from Student
    where not exists
    	(select *
        from Course
        where TNO = 'TA90023' and not exists
    		(select *
            from Score
            where Score.SNO = Student.SNO and Score.CNO = Course.CNO));
    ```

    ![image-20250415233142746](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415233142746.png)

15. 查询每门课程的最⾼分和最低分，并计算其分数差。其中每⾏包含课程号、课程名和最⾼分、最低分和分数差。（课程⽆成绩的不⽤包括）。

    ```mysql
    select Score.CNO, Course.`NAME`, max(Score.DEGREE), min(Score.DEGREE), max(Score.DEGREE)-min(Score.DEGREE)
    from Score, Course
    where Score.CNO = Course.CNO
    group by (Score.CNO);
    ```

    ![image-20250415233155001](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415233155001.png)

16. 查询存在考试成绩低于 78 分的学⽣学号，以及每个学⽣低于 78 分的课程的数量。  

    ```mysql
    select SNO, count(*)
    from Score
    where DEGREE < 78
    	/*先执行where筛选出所有DEGREE<78的元组*/
    group by SNO;
    ```

    ![image-20250415232826825](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250415232826825.png)
    
17. 查询所教过的课程中有学⽣考试成绩低于 72 分的教师的⼯号和姓名（去掉重复⾏）。  

    ```mysql
    select distinct Teacher.TNO, Teacher.`NAME`
    from Teacher, Course
    where Teacher.TNO = Course.TNO and Course.CNO in
    	(select CNO
        from Score
        where DEGREE < 72);
    ```

    ![image-20250416201554409](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250416201554409.png)

18. 查询选修⼤于 6 门课程的学⽣的学号、姓名 

    ```mysql
    select SNO, `NAME`
    from Student
    where SNO in
    	(select SNO
        from Score
        group by (SNO)
        having count(*) > 6);
    /*在任务一T11中删除了每个学生的最低分记录，故学生最多只选修了6门课*/
    ```

    ![image-20250416202015416](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250416202015416.png)

19. 查询⾄少选修了学号为SNO=”PB230000002”的同学（ZY同学）所选全部课程的学⽣学号  

    ```mysql
    select SNO
    from Student
    where not exists
    	(select *
        from Score sc1
        where SNO = 'PB230000002' and not exists
    		(select *
            from Score sc2
            where sc1.CNO = sc2.CNO and Student.SNO = sc2.SNO));
    ```

    ![image-20250416202751575](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250416202751575.png)

20. 查询student表中各个学⽣姓名与相应的平均成绩 

    ```mysql
    select Student.`NAME`, avg(Score.DEGREE)
    from Student left outer join Score on (Student.SNO = Score.SNO)	/*有一些学生没有选课，使用外连接*/
    group by Student.SNO;
    ```

    ![image-20250416203140188](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250416203140188.png)

21. 查询每个系的学⽣⼈数和每个系的平均分，其中每⾏包含系号、系的⼈数和平均成绩。（计算⼈数的时候需要包括那些没有成绩的同学，但是计算成绩的时候不需要包括这些同学）

    ```mysql
    select Student.DEPARTMENT, count(Student.SNO), avg(Score.DEGREE)
    from Student left join Score on (Student.SNO = Score.SNO)	/*有一些学生没有选课，使用外连接*/
    group by Student.DEPARTMENT;
    ```

    ![image-20250416203745636](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250416203745636.png)

22. 查询所有未选修 DB_Design 课程或者 Data_Mining 课程的学⽣的学⽣姓名（去掉重复⾏）  

    ```mysql
    select `NAME`
    from Student
    where SNO not in
    	(select SNO
        from Score
        where CNO in
    		(select CNO
            from Course
            where `NAME` = 'DB_Design' or `NAME` = 'Data_Mining'));
    ```

    ![image-20250416204310176](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250416204310176.png)

23. 查询各个课程的课程名及选该课的学⽣的平均年龄。（包括没有⼈选的课）  

    ```mysql
    select Course.`NAME`, avg_age
    from Course left join /*要求显示所有课程，使用外连接*/
    	(select Score.CNO, avg(cast(Student.AGE as signed)) as avg_age
         /*在任务一T3中年龄类型被改为char，可以进行类型转换*/
         /*AVG函数也会进行自动的类型转换*/
        from Score, Student
        where Score.SNO = Student.SNO
        group by Score.CNO) as temp on (Course.CNO = temp.CNO);
    ```

    ![image-20250416205240208](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250416205240208.png)

24. 查询选修了课程名中包含”Computer”课程的学⽣的学号和姓名  

    ```mysql
    select Student.SNO, Student.`NAME`
    from Student, Score
where Score.SNO = Student.SNO and Score.CNO in
    	(select CNO
        from Course
        where `NAME` like '%Computer%');
    ```
    
    ![image-20250417165442402](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417165442402.png)
    
25. 设课程平均成绩为 x，查询各个课程成绩处于[x-5, x+5]区间的同学的成绩表，即包含 SNO、CNO、DEGREE 

    ```mysql
    select Score.SNO, Score.CNO, Score.DEGREE
    from Score join
    	(select CNO, avg(DEGREE) as avg_degree
        from Score
        group by CNO) as course_avg on (Score.CNO = course_avg.CNO)
        /*与CNO+平均分表连接*/
    where Score.DEGREE <= course_avg.avg_degree + 5 and Score.DEGREE >= course_avg.avg_degree - 5;
    ```

    ![image-20250417170103945](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417170103945.png)

26. 查询每门课程的课程号、课程名以及在这门课程上GRADE为A的学⽣⼈数，结果按⼈数降序排列。

    ```mysql
    select Course.CNO, Course.`NAME`, A_count
    from Course join
    	(select CNO, count(*) as A_count
        from Score
        where GRADE = 'A'
        group by CNO) as Course_acount on (Course.CNO = Course_acount.CNO)
    order by A_count desc;
    ```

    ![image-20250417170510255](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417170510255.png)

27. 查询包含 " _ "（下划线）且位 " _ " 于后⾯的字符串包含"r"的所有课程的课程名。  

    ```mysql
    select `NAME`
    from Course
    where `NAME` like '%\_%r%';
    ```

    ![image-20250417170851114](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417170851114.png)

28. 查询每个⽼师教的所有课程中最⾼的平均成绩，并展⽰出对应的⽼师⼯号和课程号  

    ```mysql
    select Teacher.TNO, Course.CNO, avg_degree
    from Teacher join 
    	(Course join
    		(select CNO, avg(DEGREE) as avg_degree
    		from Score
    		group by (CNO)) as course_avg on (Course.CNO = course_avg.CNO))
    	on (Teacher.TNO = Course.TNO)
    	/*得到TNO-CNO-平均分表*/
    where (Teacher.TNO, avg_degree) in
    	(select Teacher.TNO, max(avg_degree)
        from Teacher join 
    		(Course join (select CNO, avg(DEGREE) as avg_degree
    		from Score
    		group by (CNO)) as course_avg on (Course.CNO = course_avg.CNO))
    		on (Teacher.TNO = Course.TNO)
        group by (Teacher.TNO));
        /*得到TNO-最高平均分表*/
    ```

    ![image-20250417171023098](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417171023098.png)

#### 索引

1. ⽤ create 语句在 Student 表的名称 NAME 上建⽴普通索引 NAME_INDEX  

   ```mysql
   create index NAME_INDEX
   on Student(`NAME`);
   ```

   ![image-20250417171454093](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417171454093.png)

2. ⽤ create 语句在 Course 表的课程号 CNO 上建⽴唯⼀索引 CNO_INDEX。  

   ```mysql
   create unique index CNO_INDEX
   on Course(CNO);
   ```

   ![image-20250417171604620](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417171604620.png)

3. ⽤create 语句在 Score表中按学号升序，DEGREE成绩降序建⽴索引（即先按学号升序，然后按成绩降序）  

   ```mysql
   create index SNO_DEGREE_INDEX
   on Score(SNO asc, DEGREE desc);
   ```

   ![image-20250417171739913](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417171739913.png)

4. ⽤⼀条语句查询表 score 的索引。  

   ```mysql
   show index from Score;
   ```

   ![image-20250417171950165](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417171950165.png)

5. 删除Course 表的课程号 CNO 上建⽴的唯⼀索引 CNO_INDEX  

   ```mysql
   drop index CNO_INDEX on Course;
   ```

   ![image-20250417172134374](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417172134374.png)

#### 视图

1. 建⽴ 229 系的学⽣视图（db_229_student），属性与 student 表⼀样，并要求对该视图进⾏修改和插⼊操作时仍需保证该视图只有 229 系的学⽣

   ```mysql
   create view db_229_student
   as
   	select *
       from Student
       where DEPARTMENT = 229
   with check option;
   ```

   ![image-20250417172736713](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417172736713.png)

2. 将 229 系学⽣视图（db_229_student）中学号为“PB210000031”的学⽣姓名和学号改为你的姓名和学号。  

   ```mysql
   update db_229_student
   set `NAME` = 'SF', SNO = 'PB231517990'
   where SNO = 'PB210000031';
   /*视图中不存在该学号*/
   ```

   ![image-20250417173116993](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417173116993.png)

3. 在 229 系学⽣视图（db_229_student）中找出年龄⼩于 21 岁的学⽣，包含 SNO、 NAME、GENDER  

   ```mysql
   select SNO, `NAME`, GENDER
   from db_229_student
   where AGE < 21;
   ```

   ![image-20250417173546684](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417173546684.png)

4. 向 student 表中插⼊⼀名“学号 SA242290001，姓名 QWE，性别male， 229系，年龄22”的学⽣。然后查询视图 db_229_student 的所有学⽣，验证其是否更新。

   ```mysql
   insert into db_229_student
   values('SA242290001','QWE','male','22',229);
   
   select *
   from db_229_student;
   ```

   ![image-20250417173822947](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417173822947.png)

5. 删除视图 db_229_student。  

   ```mysql
   drop view db_229_student;
   ```

   ![image-20250417173942184](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417173942184.png)

#### 触发器

1. 创建关系表： teacher_salary(TNO, SAL)，其中 TNO 是教师⼯号（主键）， SAL 是教师⼯资（类型float）。

   ```mysql
   create table teacher_salary
   	(TNO char(7) primary key,
       SAL float
       );
   ```

   ![image-20250417174529490](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417174529490.png)

2. 定义⼀个 BEFORE ⾏级触发器，为关系表 teacher_salary 定义完整性规则： “表中出现的⼯号必须也出现在teacher 表中，否则报错”。

   ```mysql
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
   ```

   ![image-20250417180421180](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417180421180.png)

   测试：插入

   ![image-20250417180702318](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417180702318.png)

   测试：修改

   ![image-20250417180824103](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417180824103.png)

3. 定义⼀个 BEFORE ⾏级触发器，为关系表 teacher_salary 定义完整性规则： “Instructor/Associate Professor/Professor 的⼯资不能低于4000/7000/10000，且不能⾼于 7000/10000/13000，如果低于则改为4000/7000/10000”，如果⾼于则改成 7000/10000/13000。

   ```mysql
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
   ```

   ![image-20250417181836667](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417181836667.png)

   测试：插入('TA90024',30000)，'TA90024'是Associate Professor，故SAL被自动修改为10000.

   ![image-20250417182001718](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417182001718.png)

   ![image-20250417182013369](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417182013369.png)

   测试：修改('TA90024',10000)为('TA90030',1)，'TA90030'是Professor，故SAL被自动修改为10000.

   ![image-20250417213820394](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417213820394.png)

   ![image-20250417213835146](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417213835146.png)

4. 删除刚刚创建的所有触发器。  

   ```mysql
   drop trigger TS_T1;
   drop trigger TS_T2;
   drop trigger TS_T3;
   drop trigger TS_T4;
   ```
   
   ![image-20250417215411879](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417215411879.png)

#### 空值

1. 将 score 表中的DB_Design课程成绩设为空值，然后在 score 表查询选修此课学⽣学号和分数，并按分数升序展⽰。观察 NULL 在 MySQL 中的⼤⼩是怎样的？

   ```mysql
   update Score
   set DEGREE = NULL
   where CNO = 
   	(select CNO
       from Course
       where `NAME` = 'DB_Design');
       
   /*查询选修此刻学生学号和其选修过的所有课程分数，否则升序展示无法显示出NULL的大小*/
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
   ```
   
   ![image-20250417220403380](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417220403380.png)
   
   即NULL在MySQL中被设为最小值。

#### 开放题

请⾃⼰设计两个题⽬并给出sql代码实现。

1. 基于Score.csv文件建立Score_new表（避免之前题目中删除的分数数据影响），添加属性GPA，并基于科大的标准计算所有记录的GPA。

   ```mysql
   create table Score_new
   	(SNO char(11),
       CNO char(8),
   	DEGREE int,
   	SEMESTER char(6),
       primary key(SNO,CNO),
   	foreign key (SNO) references Student(SNO),
       foreign key (CNO) references Course(CNO)
       );
   
   alter table Score_new
   add GPA float;
   
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
   ```

   ![image-20250417224537014](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417224537014.png)

   ![image-20250417224725988](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417224725988.png)

2. 为Student表添加属性总学分CREDIT、加权平均分AVG_SCORE、总绩点GPA，并基于Student、Course、Score_new表的内容填充，加权平均分和GPA的计算按照科大的评分标准。最后展示每个专业的GPA第一名（输出Student表里的各项数据）。

   ```mysql
   alter table Student
   add CREDIT float,
   add AVG_SCORE float,
   add GPA float;
   
   /*计算总学分*/
   update Student
   set CREDIT = 
   	(select sum(CCREDIT)
       from Course, Score_new
       where Course.CNO = Score_new.CNO and Score_new.SNO = Student.SNO);
   /*计算加权平均分*/    
   update Student
   set AVG_SCORE = 
   	(select sum(Score_new.DEGREE * Course.CCREDIT)/Student.CREDIT
       from Course, Score_new
       where Course.CNO = Score_new.CNO and Score_new.SNO = Student.SNO);
   /*计算GPA*/    
   update Student
   set GPA = 
   	(select sum(Score_new.GPA * Course.CCREDIT)/Student.CREDIT
       from Course, Score_new
       where Course.CNO = Score_new.CNO and Score_new.SNO = Student.SNO);
   ```

   ![image-20250417230657367](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417230657367.png)

   ```mysql
   /*展示每个专业的GPA第一名*/
   select *
   from Student
   where (DEPARTMENT, GPA) in
   	(select DEPARTMENT, max(GPA)
       from Student
       group by DEPARTMENT);
   ```

   ![image-20250417230629007](C:\Users\时分\AppData\Roaming\Typora\typora-user-images\image-20250417230629007.png)

