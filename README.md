<!--
 * @Author: Scorpiour dongchen_101@hotmail.com
 * @Date: 2022-11-27 22:56:17
 * @LastEditors: Scorpiour dongchen_101@hotmail.com
 * @LastEditTime: 2022-11-27 22:56:17
 * @FilePath: /brix-activity-watch/README.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
Provided by: Chris, Qihang
Date: Oct 17th, 2019
Version: 1.0

1.	How to check records from the Brix Labs Activity Watch.
1.	Go to address project.joinbrix.com:4433. The account is
1.1.	Username: brix@joinbrix.com
1.2.	Password: Brix1234!
2.	Select sever/ activity watch. Enter password Brix1234!
3.	Select activity watch/ postgres/ Schemas/ public/ table in the left side bar.
4.	Right-click “records” and select “query tool”. Click on “Open files” in the main window. Functions will be shown.
5.	The main functions to be used are introduced as follows:
5.1.	get_all_records_for_dev_between_two_dates.sql:
change parameters in config (line 3): dev_email, start_date, end_date to the needed user and duration. Click on the “Execute/Refresh” or F5 to execute the query.
5.2.	get_work_hours_for_dev_between_two_dates.sql:
change parameters in config (line 3): dev_email, start_date, end_date to the needed user and duration. Click on the “Excute/Refresh” or F5 to excute the query.
2.	Installing Brix Labs Activity Watch.
1.	Go to Brix Platform Development/ Brix-ActivityWatch/ Build/ Mar31
2.	For Windows users, download the “Windows” folder as a zip file and send. The windows users will install the package.
3.	For Mac users, download the “BrixActivityWatch.dmg” file in the Mac folder and send.
