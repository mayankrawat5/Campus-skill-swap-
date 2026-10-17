# Campus Skill Swap Network

A modular C++ implementation of a skill exchange platform for college students, designed as a DSA/OOP project.

## Project Structure

```
Campus-Skill-Swap/
├── include/
│   ├── skill.h
│   ├── student.h
│   ├── matching_engine.h
│   ├── exchange_request.h
│   ├── scheduler.h
│   ├── rating.h
│   ├── notification.h
│   ├── skill_graph.h
│   ├── admin.h
│   └── campus_system.h
│
├── src/
│   ├── skill.cpp
│   ├── student.cpp
│   ├── matching_engine.cpp
│   ├── exchange_request.cpp
│   ├── scheduler.cpp
│   ├── rating.cpp
│   ├── notification.cpp
│   ├── skill_graph.cpp
│   ├── admin.cpp
│   └── campus_system.cpp
│
├── tests/
│   ├── student_test.cpp
│   ├── exchange_request_test.cpp
│   ├── scheduler_test.cpp
│   └── ... (additional test files)
│
├── demo/
│   └── campus_skill_swap_demo.cpp
│
└── README.md
```

## Key Features

- **Modular Design**: Separated into distinct components following OOP principles
- **Data Structures & Algorithms**:
  - Queue-based notification system (FIFO)
  - Static counters for deterministic ID generation (REQ1, REQ2..., EX1, EX2...)
  - Skill evolution map using graph(BFS) for learning path finding
  - Compatibility scoring algorithm (40% skill, 20% mode, 15% location, 15% availability, 10% rating)
- **Object Lifecycle Management**: Proper state machines for ExchangeRequest and ScheduledExchange
- **Encapsulation**: Clean separation of concerns between modules

## Fixes Applied During Refactoring

1. **Notification System**: Changed from `std::vector<Notification>` to `std::queue<Notification>` with proper FIFO operations
2. **Request IDs**: Replaced `rand() % 10000` with deterministic IDs using static counters (REQ1, REQ2, REQ3...)
3. **Scheduled Exchange IDs**: Replaced `rand() % 10000` with deterministic IDs using static counters (EX1, EX2, EX3...)
4. **Exchange Request Bug**: Fixed to modify original objects in `exchangeRequests` vector using references/ID lookup
5. **Exchange Lifecycle**: Kept state machines logically separate:
   - ExchangeRequest: Pending → Accepted/Rejected/Cancelled
   - ScheduledExchange: Scheduled → Completed/Cancelled
6. **Completed Exchange Count**: Only incremented when ScheduledExchange actually becomes Completed

## Building and Running

### Compile the Demo Program
```bash
g++ -std=c++11 -Wall -Wextra -o campus_skill_swap_demo demo/campus_skill_swap_demo.cpp src/*.cpp
```

### Run the Program
```bash
./campus_skill_swap_demo
```

### Compile and Run Tests
```bash
# Student tests
g++ -std=c++11 -Wall -Wextra -o student_test tests/student_test.cpp src/student.cpp
./student_test

# Exchange request tests
g++ -std=c++11 -Wall -Wextra -o exchange_request_test tests/exchange_request_test.cpp src/exchange_request.cpp
./exchange_request_test

# Scheduler tests
g++ -std=c++11 -Wall -Wextra -o scheduler_test tests/scheduler_test.cpp src/scheduler.cpp
./scheduler_test
```

## Design Notes

- Each module follows the header → source → test → compile → run → verify workflow
- Minimal dependencies - only uses C++ standard library
- Designed for educational purposes to demonstrate OOP and DSA concepts
- Preserves all functionality from the original monolithic implementation