WITH
    FIRST_LEVEL_EMPLOYEES as (
        SELECT EMPLOYEE_ID, DEPARTMENT_ID
        FROM EMPLOYEES
        WHERE MANAGER_ID is NULL
    ),

    FIRST_LEVEL_PER_DEPARTMENT as (
        SELECT D.DEPARTMENT_ID, NVL2(E.EMPLOYEE_ID, count(*) OVER(PARTITION BY D.DEPARTMENT_ID), 0) as FIRST_LEVEL
        FROM DEPARTMENTS D
            left join FIRST_LEVEL_EMPLOYEES E on D.DEPARTMENT_ID = E.DEPARTMENT_ID
        ORDER BY D.DEPARTMENT_ID
    ),

    SECOND_LEVEL_EMPLOYEES as (
        SELECT E1.EMPLOYEE_ID, E1.DEPARTMENT_ID
        FROM EMPLOYEES E1
            join FIRST_LEVEL_EMPLOYEES E2 on E1.MANAGER_ID = E2.EMPLOYEE_ID
    ),

    SECOND_LEVEL_PER_DEPARTMENT as (
        SELECT D.DEPARTMENT_ID, NVL2(E.EMPLOYEE_ID, count(*) OVER(PARTITION BY D.DEPARTMENT_ID), 0) as SECOND_LEVEL
        FROM DEPARTMENTS D
            left join SECOND_LEVEL_EMPLOYEES E on D.DEPARTMENT_ID = E.DEPARTMENT_ID
        ORDER BY D.DEPARTMENT_ID
    ),

    THIRD_LEVEL_EMPLOYEES as (
        SELECT E1.EMPLOYEE_ID, E1.DEPARTMENT_ID
        FROM EMPLOYEES E1
            join SECOND_LEVEL_EMPLOYEES E2 on E1.MANAGER_ID = E2.EMPLOYEE_ID
    ),

    THIRD_LEVEL_PER_DEPARTMENT as (
        SELECT D.DEPARTMENT_ID, NVL2(E.EMPLOYEE_ID, count(*) OVER(PARTITION BY D.DEPARTMENT_ID), 0) as THIRD_LEVEL
        FROM DEPARTMENTS D
            left join THIRD_LEVEL_EMPLOYEES E on D.DEPARTMENT_ID = E.DEPARTMENT_ID
        ORDER BY D.DEPARTMENT_ID
    )

SELECT DISTINCT D.DEPARTMENT_NAME, F.FIRST_LEVEL, S.SECOND_LEVEL, T.THIRD_LEVEL
FROM DEPARTMENTS D
    join FIRST_LEVEL_PER_DEPARTMENT F on D.DEPARTMENT_ID = F.DEPARTMENT_ID
    join SECOND_LEVEL_PER_DEPARTMENT S on D.DEPARTMENT_ID = S.DEPARTMENT_ID
    join THIRD_LEVEL_PER_DEPARTMENT T on D.DEPARTMENT_ID = T.DEPARTMENT_ID
    join LOCATIONS using (LOCATION_ID)
    join COUNTRIES using (COUNTRY_ID)
    join REGIONS using (REGION_ID)
WHERE REGION_NAME = 'Americas'
ORDER BY D.DEPARTMENT_NAME
