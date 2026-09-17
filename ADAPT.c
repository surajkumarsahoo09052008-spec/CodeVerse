

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STUDENTS 50
#define MAX_TOPICS 10
#define MAX_QUESTIONS 30
#define MAX_PATH 10

/* ----------------------------- DATA TYPES ----------------------------- */

typedef struct {
    int id;
    char name[50];
    char username[50];
    char password[50];

    char branch[50];
    int semester;

    char careerGoal[100];
    int studyTime;

    int assessmentPerformance[MAX_TOPICS];
    int practicePerformance[MAX_TOPICS];
    int recentPerformance[MAX_TOPICS];
    int consistency[MAX_TOPICS];

    float mastery[MAX_TOPICS];
    float retention[MAX_TOPICS];

    int questionsSolved;
    int topicsCovered;
} Student;

typedef struct {
    int id;
    char topic[50];
    char question[250];

    char optionA[100];
    char optionB[100];
    char optionC[100];
    char optionD[100];

    char correctAnswer;

    int difficulty;
} Question;

typedef struct {
    char name[50];
    char prerequisite[50];
} Topic;

/* ----------------------------- GLOBAL DATA ----------------------------- */

Student students[MAX_STUDENTS];
int studentCount = 0;

Topic topics[MAX_TOPICS] = {
    {"Programming Fundamentals", "None"},
    {"Arrays", "Programming Fundamentals"},
    {"Linked Lists", "Arrays"},
    {"Stacks", "Linked Lists"},
    {"Queues", "Linked Lists"},
    {"Trees", "Stacks"},
    {"Binary Search Trees", "Trees"},
    {"Graphs", "Trees"},
    {"DBMS", "Programming Fundamentals"},
    {"Computer Networks", "Programming Fundamentals"}
};

Question questions[MAX_QUESTIONS] = {

    {1, "Programming Fundamentals",
     "Which symbol is used to end a statement in C?",
     ";", ":", ".", ",", 'A', 1},

    {2, "Programming Fundamentals",
     "Which data type stores an integer?",
     "float", "int", "char", "double", 'B', 1},

    {3, "Arrays",
     "Array indexing in C starts from:",
     "0", "1", "-1", "2", 'A', 1},

    {4, "Arrays",
     "Which is used to access an array element?",
     "Index", "Pointer only", "Function", "Loop", 'A', 1},

    {5, "Arrays",
     "What is the time complexity of accessing an array element by index?",
     "O(n)", "O(log n)", "O(1)", "O(n²)", 'C', 2},

    {6, "Linked Lists",
     "A linked list consists of:",
     "Nodes", "Only arrays", "Only functions", "Only loops", 'A', 1},

    {7, "Linked Lists",
     "Which field stores the address of the next node?",
     "data", "next", "value", "index", 'B', 1},

    {8, "Stacks",
     "Which principle does a stack follow?",
     "FIFO", "LIFO", "Random", "Priority", 'B', 1},

    {9, "Queues",
     "Which principle does a queue follow?",
     "LIFO", "FIFO", "Random", "Binary", 'B', 1},

    {10, "Trees",
     "A binary tree node can have at most:",
     "1 child", "2 children", "3 children", "4 children", 'B', 1},

    {11, "Trees",
     "Which traversal is commonly used for a binary search tree to obtain sorted order?",
     "Preorder", "Postorder", "Inorder", "Level order", 'C', 2},

    {12, "Binary Search Trees",
     "In a BST, values smaller than the root are generally stored:",
     "Right", "Left", "Above", "Randomly", 'B', 2},

    {13, "Graphs",
     "A graph consists primarily of:",
     "Vertices and edges", "Arrays and stacks",
     "Functions and loops", "Nodes only", 'A', 1},

    {14, "Graphs",
     "BFS commonly uses which data structure?",
     "Stack", "Queue", "Heap only", "Array only", 'B', 2},

    {15, "Graphs",
     "DFS commonly uses:",
     "Stack/recursion", "Queue only", "Database", "Hash table only", 'A', 2},

    {16, "DBMS",
     "DBMS stands for:",
     "Database Management System",
     "Data Binary Management System",
     "Database Machine System",
     "Data Management Software",
     'A', 1},

    {17, "DBMS",
     "Which language is commonly used to query relational databases?",
     "HTML", "SQL", "CSS", "C", 'B', 1},

    {18, "DBMS",
     "A primary key must uniquely identify:",
     "A table", "A row", "A database server", "A query", 'B', 2},

    {19, "Computer Networks",
     "IP is primarily used for:",
     "Addressing", "Sorting", "Compilation", "Encryption only", 'A', 1},

    {20, "Computer Networks",
     "Which protocol is connection-oriented?",
     "UDP", "TCP", "IP", "ARP", 'B', 2}
};

/* ----------------------------- UTILITY FUNCTIONS ----------------------------- */

void clearInputBuffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void pauseScreen(void)
{
    printf("\nPress ENTER to continue...");
    getchar();
}

int findStudent(const char username[])
{
    int i;

    for (i = 0; i < studentCount; i++) {
        if (strcmp(students[i].username, username) == 0)
            return i;
    }

    return -1;
}

void calculateMastery(Student *s, int topic)
{
    /*
        Mastery =
        50% Assessment
        20% Practice
        15% Recent Performance
        15% Consistency
    */

    s->mastery[topic] =
        (0.50f * s->assessmentPerformance[topic]) +
        (0.20f * s->practicePerformance[topic]) +
        (0.15f * s->recentPerformance[topic]) +
        (0.15f * s->consistency[topic]);
}

const char *masteryLevel(float value)
{
    if (value < 40)
        return "Beginner";

    if (value < 60)
        return "Developing";

    if (value < 80)
        return "Proficient";

    return "Advanced";
}

int topicIndex(const char topicName[])
{
    int i;

    for (i = 0; i < MAX_TOPICS; i++) {
        if (strcmp(topics[i].name, topicName) == 0)
            return i;
    }

    return -1;
}

void initializeStudent(Student *s)
{
    int i;

    for (i = 0; i < MAX_TOPICS; i++) {
        s->assessmentPerformance[i] = 50;
        s->practicePerformance[i] = 50;
        s->recentPerformance[i] = 50;
        s->consistency[i] = 50;

        s->mastery[i] = 50;
        s->retention[i] = 50;
    }

    s->questionsSolved = 0;
    s->topicsCovered = 0;
}

/* ----------------------------- CREATE ACCOUNT ----------------------------- */

void createAccount(void)
{
    Student newStudent;

    if (studentCount >= MAX_STUDENTS) {
        printf("\nStudent database is full.\n");
        return;
    }

    initializeStudent(&newStudent);

    newStudent.id = studentCount + 1;

    printf("\n====================================\n");
    printf("       CREATE NEW ACCOUNT\n");
    printf("====================================\n");

    printf("Full Name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';

    printf("Username: ");
    fgets(newStudent.username, sizeof(newStudent.username), stdin);
    newStudent.username[strcspn(newStudent.username, "\n")] = '\0';

    if (findStudent(newStudent.username) != -1) {
        printf("\nUsername already exists.\n");
        return;
    }

    printf("Password: ");
    fgets(newStudent.password, sizeof(newStudent.password), stdin);
    newStudent.password[strcspn(newStudent.password, "\n")] = '\0';

    printf("Branch: ");
    fgets(newStudent.branch, sizeof(newStudent.branch), stdin);
    newStudent.branch[strcspn(newStudent.branch, "\n")] = '\0';

    printf("Semester: ");
    scanf("%d", &newStudent.semester);
    clearInputBuffer();

    printf("Career Goal: ");
    fgets(newStudent.careerGoal, sizeof(newStudent.careerGoal), stdin);
    newStudent.careerGoal[strcspn(newStudent.careerGoal, "\n")] = '\0';

    printf("Available study time per day (minutes): ");
    scanf("%d", &newStudent.studyTime);
    clearInputBuffer();

    students[studentCount] = newStudent;
    studentCount++;

    printf("\nAccount created successfully!\n");
    printf("Welcome to AdaptiveLearn, %s!\n", newStudent.name);
}

/* ----------------------------- LOGIN ----------------------------- */

int login(void)
{
    char username[50];
    char password[50];
    int index;

    printf("\n====================================\n");
    printf("          ADAPTIVELEARN LOGIN\n");
    printf("====================================\n");

    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    index = findStudent(username);

    if (index == -1) {
        printf("\nAccount not found.\n");
        return -1;
    }

    if (strcmp(students[index].password, password) != 0) {
        printf("\nIncorrect password.\n");
        return -1;
    }

    printf("\nLogin successful!\n");
    printf("Welcome back, %s!\n", students[index].name);

    return index;
}

/* ----------------------------- PROFILE ----------------------------- */

void showProfile(Student *s)
{
    printf("\n====================================\n");
    printf("             PROFILE\n");
    printf("====================================\n");

    printf("Name          : %s\n", s->name);
    printf("Username      : %s\n", s->username);
    printf("Branch        : %s\n", s->branch);
    printf("Semester      : %d\n", s->semester);
    printf("Career Goal   : %s\n", s->careerGoal);
    printf("Study Time    : %d minutes/day\n", s->studyTime);
}

/* ----------------------------- DIAGNOSTIC TEST ----------------------------- */

void diagnosticTest(Student *s)
{
    int i;
    int score[MAX_TOPICS] = {0};
    int total[MAX_TOPICS] = {0};
    int answer;
    int qCount = 20;

    printf("\n====================================\n");
    printf("          DIAGNOSTIC TEST\n");
    printf("====================================\n");

    printf("Answer the following questions.\n");

    for (i = 0; i < qCount; i++) {

        Question *q = &questions[i];

        printf("\nQuestion %d\n", i + 1);
        printf("Topic: %s\n", q->topic);
        printf("%s\n\n", q->question);

        printf("A. %s\n", q->optionA);
        printf("B. %s\n", q->optionB);
        printf("C. %s\n", q->optionC);
        printf("D. %s\n", q->optionD);

        printf("\nYour answer: ");
        scanf("%d", &answer);
        clearInputBuffer();

        char selected;

        if (answer == 1)
            selected = 'A';
        else if (answer == 2)
            selected = 'B';
        else if (answer == 3)
            selected = 'C';
        else
            selected = 'D';

        int index = topicIndex(q->topic);

        if (index >= 0) {

            total[index]++;

            if (selected == q->correctAnswer)
                score[index]++;
        }
    }

    for (i = 0; i < MAX_TOPICS; i++) {

        if (total[i] > 0) {
            s->assessmentPerformance[i] =
                (score[i] * 100) / total[i];

            s->recentPerformance[i] =
                s->assessmentPerformance[i];

            calculateMastery(s, i);
        }
    }

    printf("\nDiagnostic completed successfully!\n");

    pauseScreen();
}

/* ----------------------------- SKILL MAP ----------------------------- */

void showSkillMap(Student *s)
{
    int i;

    printf("\n====================================\n");
    printf("             SKILL MAP\n");
    printf("====================================\n");

    printf("%-28s %-10s %-15s %-12s\n",
           "Topic", "Mastery", "Level", "Retention");

    printf("---------------------------------------------------------------\n");

    for (i = 0; i < MAX_TOPICS; i++) {

        printf("%-28s %6.1f%%   %-15s %6.1f%%\n",
               topics[i].name,
               s->mastery[i],
               masteryLevel(s->mastery[i]),
               s->retention[i]);
    }

    pauseScreen();
}

/* ----------------------------- SKILL GAP DETECTION ----------------------------- */

void skillGapAnalysis(Student *s)
{
    int i;

    printf("\n====================================\n");
    printf("          SKILL GAP ANALYSIS\n");
    printf("====================================\n");

    for (i = 0; i < MAX_TOPICS; i++) {

        if (s->mastery[i] < 60) {

            printf("\nWeak Skill: %s\n", topics[i].name);
            printf("Mastery: %.1f%%\n", s->mastery[i]);
            printf("Status: Needs Attention\n");
            printf("Prerequisite: %s\n", topics[i].prerequisite);

            if (s->mastery[i] < 40)
                printf("Recommendation: Reinforce fundamentals.\n");
            else
                printf("Recommendation: Practice more questions.\n");
        }
    }

    pauseScreen();
}

/* ----------------------------- LEARNING PATH ----------------------------- */

void generateLearningPath(Student *s)
{
    int i;

    printf("\n====================================\n");
    printf("        PERSONALIZED LEARNING PATH\n");
    printf("====================================\n");

    printf("\nRecommended sequence:\n\n");

    int count = 0;

    /*
        Weak topics are prioritized.
        Very weak topics come first.
    */

    for (i = 0; i < MAX_TOPICS; i++) {

        if (s->mastery[i] < 60) {

            count++;

            printf("%d. %s\n", count, topics[i].name);

            printf("   Current mastery: %.1f%%\n",
                   s->mastery[i]);

            printf("   Why:");

            if (s->mastery[i] < 40)
                printf("   Your mastery is below 40%%, so fundamentals need reinforcement.\n");
            else
                printf("   Additional practice can improve this developing skill.\n");

            printf("   Prerequisite: %s\n\n",
                   topics[i].prerequisite);
        }
    }

    if (count == 0)
        printf("Excellent! No major skill gaps detected.\n");

    pauseScreen();
}

/* ----------------------------- PRACTICE ----------------------------- */

void practice(Student *s)
{
    int number;
    int i;
    int answer;
    int correct = 0;

    printf("\n====================================\n");
    printf("             PRACTICE\n");
    printf("====================================\n");

    printf("Choose number of questions (5 / 10 / 15): ");
    scanf("%d", &number);
    clearInputBuffer();

    if (number != 5 && number != 10 && number != 15) {
        printf("Invalid choice. Using 5 questions.\n");
        number = 5;
    }

    if (number > MAX_QUESTIONS)
        number = MAX_QUESTIONS;

    for (i = 0; i < number; i++) {

        Question *q = &questions[i];

        printf("\nQuestion %d of %d\n", i + 1, number);
        printf("Topic: %s\n", q->topic);
        printf("Difficulty: %s\n",
               q->difficulty == 1 ? "Easy" : "Medium");

        printf("\n%s\n", q->question);

        printf("1. %s\n", q->optionA);
        printf("2. %s\n", q->optionB);
        printf("3. %s\n", q->optionC);
        printf("4. %s\n", q->optionD);

        printf("\nAnswer: ");
        scanf("%d", &answer);
        clearInputBuffer();

        char selected;

        if (answer == 1)
            selected = 'A';
        else if (answer == 2)
            selected = 'B';
        else if (answer == 3)
            selected = 'C';
        else
            selected = 'D';

        int index = topicIndex(q->topic);

        if (selected == q->correctAnswer) {

            printf("Correct! ?\n");

            correct++;

            if (index >= 0) {

                s->practicePerformance[index] += 10;

                if (s->practicePerformance[index] > 100)
                    s->practicePerformance[index] = 100;

                s->recentPerformance[index] += 5;

                if (s->recentPerformance[index] > 100)
                    s->recentPerformance[index] = 100;

                calculateMastery(s, index);
            }

        } else {

            printf("Incorrect. ?\n");
            printf("Correct answer: %c\n",
                   q->correctAnswer);

            if (index >= 0) {

                s->practicePerformance[index] -= 5;

                if (s->practicePerformance[index] < 0)
                    s->practicePerformance[index] = 0;

                calculateMastery(s, index);
            }
        }

        s->questionsSolved++;
    }

    printf("\n====================================\n");
    printf("Practice Score: %d/%d\n", correct, number);
    printf("====================================\n");

    updateTopicsCovered(s);

    pauseScreen();
}

/* ----------------------------- ADAPTIVE DIFFICULTY ----------------------------- */

void adaptiveDifficulty(Student *s)
{
    int i;

    printf("\n====================================\n");
    printf("         ADAPTIVE DIFFICULTY\n");
    printf("====================================\n");

    for (i = 0; i < MAX_TOPICS; i++) {

        printf("\n%s\n", topics[i].name);
        printf("Mastery: %.1f%%\n", s->mastery[i]);

        if (s->mastery[i] >= 80) {

            printf("Action: Increase difficulty ??\n");
            printf("Next: Advanced problems\n");

        } else if (s->mastery[i] >= 60) {

            printf("Action: Maintain difficulty\n");
            printf("Next: Standard practice\n");

        } else if (s->mastery[i] >= 40) {

            printf("Action: Additional practice ??\n");
            printf("Next: More guided questions\n");

        } else {

            printf("Action: Reinforce prerequisites ??\n");
            printf("Next: Fundamentals + easy practice\n");
        }
    }

    pauseScreen();
}

/* ----------------------------- REVISION / RETENTION ----------------------------- */

void revisionTest(Student *s)
{
    int i;
    int answer;
    int score;

    printf("\n====================================\n");
    printf("       TWO-WEEK REVISION TEST\n");
    printf("====================================\n");

    printf("This test checks long-term retention.\n");

    for (i = 0; i < MAX_TOPICS; i++) {

        if (s->mastery[i] >= 40) {

            printf("\nTopic: %s\n", topics[i].name);

            printf("Enter your simulated revision score (0-100): ");

            scanf("%d", &answer);
            clearInputBuffer();

            if (answer < 0)
                answer = 0;

            if (answer > 100)
                answer = 100;

            score = answer;

            s->retention[i] = score;

            s->recentPerformance[i] = score;

            calculateMastery(s, i);
        }
    }

    printf("\nRevision analysis complete.\n");

    for (i = 0; i < MAX_TOPICS; i++) {

        if (s->retention[i] < 60) {

            printf("\n? %s has low retention: %.1f%%\n",
                   topics[i].name,
                   s->retention[i]);

            printf("Action: Concept recap + targeted practice + reassessment.\n");
        }
    }

    pauseScreen();
}

/* ----------------------------- UPDATE TOPICS COVERED ----------------------------- */

void updateTopicsCovered(Student *s)
{
    int i;
    s->topicsCovered = 0;

    for (i = 0; i < MAX_TOPICS; i++) {

        if (s->mastery[i] >= 60)
            s->topicsCovered++;
    }
}

/* ----------------------------- PROGRESS ----------------------------- */

void progress(Student *s)
{
    int i;
    float total = 0;

    updateTopicsCovered(s);

    for (i = 0; i < MAX_TOPICS; i++)
        total += s->mastery[i];

    total /= MAX_TOPICS;

    printf("\n====================================\n");
    printf("              PROGRESS\n");
    printf("====================================\n");

    printf("Overall Mastery : %.1f%%\n", total);
    printf("Topics Covered  : %d/%d\n",
           s->topicsCovered, MAX_TOPICS);
    printf("Questions Solved: %d\n", s->questionsSolved);

    printf("\nTopic Progress:\n");

    for (i = 0; i < MAX_TOPICS; i++) {

        printf("%-28s %.1f%%\n",
               topics[i].name,
               s->mastery[i]);
    }

    pauseScreen();
}

/* ----------------------------- LEADERBOARD ----------------------------- */

int calculatePoints(Student *s)
{
    updateTopicsCovered(s);

    return (100 * s->topicsCovered) +
           (2 * s->questionsSolved);
}

void leaderboard(void)
{
    int i, j;

    Student temp;

    for (i = 0; i < studentCount - 1; i++) {

        for (j = i + 1; j < studentCount; j++) {

            if (calculatePoints(&students[j]) >
                calculatePoints(&students[i])) {

                temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }

    printf("\n====================================\n");
    printf("             LEADERBOARD\n");
    printf("====================================\n");

    printf("%-5s %-20s %-15s %-10s\n",
           "Rank", "Student", "Topics", "Points");

    for (i = 0; i < studentCount; i++) {

        printf("%-5d %-20s %-15d %-10d\n",
               i + 1,
               students[i].name,
               students[i].topicsCovered,
               calculatePoints(&students[i]));
    }

    pauseScreen();
}

/* ----------------------------- AI TUTOR SIMULATION ----------------------------- */

void aiTutor(Student *s)
{
    char topic[100];

    printf("\n====================================\n");
    printf("              AI TUTOR\n");
    printf("====================================\n");

    printf("Ask about a topic: ");

    fgets(topic, sizeof(topic), stdin);
    topic[strcspn(topic, "\n")] = '\0';

    printf("\n?? Thinking about the topic...\n");
    printf("?? Finding the key concept...\n");
    printf("?? Building a simple explanation...\n");
    printf("?? Adding a practical example...\n");
    printf("? Explanation ready!\n\n");

    printf("Topic: %s\n\n", topic);

    printf("Adaptive explanation:\n");

    printf("Your tutor would explain \"%s\" according to your current\n",
           topic);

    printf("mastery level and provide an example followed by practice.\n");

    printf("\nCurrent learning goal: %s\n",
           s->careerGoal);

    pauseScreen();
}

/* ----------------------------- DASHBOARD ----------------------------- */

void dashboard(Student *s)
{
    int choice;

    do {

        printf("\n\n============================================\n");
        printf("              ADAPTIVELEARN\n");
        printf("============================================\n");

        printf("?? Welcome, %s!\n", s->name);

        printf("\n1. Profile");
        printf("\n2. Diagnostic Test");
        printf("\n3. Skill Map");
        printf("\n4. Skill Gap Analysis");
        printf("\n5. Personalized Learning Path");
        printf("\n6. Practice");
        printf("\n7. Adaptive Difficulty");
        printf("\n8. Two-Week Revision");
        printf("\n9. Progress");
        printf("\n10. Leaderboard");
        printf("\n11. AI Tutor");
        printf("\n12. Logout");

        printf("\n\nChoose option: ");

        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {

            case 1:
                showProfile(s);
                break;

            case 2:
                diagnosticTest(s);
                break;

            case 3:
                showSkillMap(s);
                break;

            case 4:
                skillGapAnalysis(s);
                break;

            case 5:
                generateLearningPath(s);
                break;

            case 6:
                practice(s);
                break;

            case 7:
                adaptiveDifficulty(s);
                break;

            case 8:
                revisionTest(s);
                break;

            case 9:
                progress(s);
                break;

            case 10:
                leaderboard();
                break;

            case 11:
                aiTutor(s);
                break;

            case 12:
                printf("\nLogging out...\n");
                printf("?? Goodbye, %s!\n", s->name);
                break;

            default:
                printf("\nInvalid option.\n");
        }

    } while (choice != 12);
}

/* ----------------------------- DEMO ACCOUNTS ----------------------------- */

void loadDemoAccounts(void)
{
    Student a, b, c;

    initializeStudent(&a);

    a.id = 1;
    strcpy(a.name, "Student A");
    strcpy(a.username, "student.a");
    strcpy(a.password, "demo123");
    strcpy(a.branch, "B.Tech CSE");
    a.semester = 3;
    strcpy(a.careerGoal, "Technical Interview");
    a.studyTime = 60;

    initializeStudent(&b);

    b.id = 2;
    strcpy(b.name, "Student B");
    strcpy(b.username, "student.b");
    strcpy(b.password, "demo123");
    strcpy(b.branch, "B.Tech CSE");
    b.semester = 3;
    strcpy(b.careerGoal, "Placement Preparation");
    b.studyTime = 90;

    initializeStudent(&c);

    c.id = 3;
    strcpy(c.name, "Student C");
    strcpy(c.username, "student.c");
    strcpy(c.password, "demo123");
    strcpy(c.branch, "B.Tech CSE");
    c.semester = 4;
    strcpy(c.careerGoal, "Semester Examination");
    c.studyTime = 45;

    /* Student A: strong arrays/linked lists, weak trees/graphs */

    a.assessmentPerformance[1] = 90;
    a.assessmentPerformance[2] = 85;
    a.assessmentPerformance[3] = 82;
    a.assessmentPerformance[5] = 45;
    a.assessmentPerformance[7] = 35;

    /* Student B: strong trees/graphs, weak fundamentals/arrays */

    b.assessmentPerformance[0] = 45;
    b.assessmentPerformance[1] = 42;
    b.assessmentPerformance[2] = 50;
    b.assessmentPerformance[5] = 80;
    b.assessmentPerformance[7] = 75;

    /* Student C: high mastery but low retention */

    c.assessmentPerformance[5] = 82;
    c.assessmentPerformance[6] = 80;
    c.assessmentPerformance[7] = 78;

    int i;

    for (i = 0; i < MAX_TOPICS; i++) {

        a.practicePerformance[i] = a.assessmentPerformance[i];
        a.recentPerformance[i] = a.assessmentPerformance[i];

        b.practicePerformance[i] = b.assessmentPerformance[i];
        b.recentPerformance[i] = b.assessmentPerformance[i];

        c.practicePerformance[i] = c.assessmentPerformance[i];
        c.recentPerformance[i] = c.assessmentPerformance[i];

        calculateMastery(&a, i);
        calculateMastery(&b, i);
        calculateMastery(&c, i);
    }

    c.retention[5] = 48;
    c.retention[6] = 52;
    c.retention[7] = 56;

    students[studentCount++] = a;
    students[studentCount++] = b;
    students[studentCount++] = c;
}

/* ----------------------------- MAIN ----------------------------- */

int main(void)
{
    int choice;
    int loggedInStudent;

    loadDemoAccounts();

    printf("\n");
    printf("============================================\n");
    printf("       ADAPTIVELEARN - AI LEARNING\n");
    printf("============================================\n");

    do {

        printf("\n1. Login");
        printf("\n2. Create New Account");
        printf("\n3. Exit");

        printf("\n\nChoose option: ");

        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {

            case 1:

                loggedInStudent = login();

                if (loggedInStudent != -1)
                    dashboard(&students[loggedInStudent]);

                break;

            case 2:
                createAccount();
                break;

            case 3:
                printf("\nThank you for using AdaptiveLearn!\n");
                break;

            default:
                printf("\nInvalid option.\n");
        }

    } while (choice != 3);

    return 0;
}
