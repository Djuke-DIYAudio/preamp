// Event handler code obtained from
// http://www.codeproject.com/Articles/34675/C-Programming-for-Embedded-System

/* Application event handler function pointer */
typedef void (*tEventHandler)();

/* Link-list definition */
typedef struct TaskRecord
{
	unsigned char EventID;
	tEventHandler EventHandler;
} tTaskRecord;

typedef struct EventRecord
{
    unsigned char EventID;
    struct EventRecord *pNext;
} tEventRecord;

// Function prototpyes
void RegisterEvent(unsigned char EventID, tEventHandler EventHandlerFunc);
void DeRegisterEvent(unsigned char EventID);
void EventManager(void);
void TriggerEvent(unsigned char EventID);