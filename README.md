# Unreal Engine 5 – Modular Inventory System

A modular Inventory & Hotbar system built in C++ and Blueprint for Unreal Engine 5.5.  

---

## 🎮 Features

- **Inventory Panel (5×2)**  
  Opens with `E`, full mouse interaction  
  - Drag & Drop between slots  
  - Stacking merge logic  
  - Right-click to use consumables  
- **Hotbar (1×5)**  
  Always visible and usable in gameplay  
  - Select with number keys `1–5`  
  - Dedicated active-slot highlight  
  - Quick use support (health potion example)

---

## 🧩 Architecture

- **UInventoryComponent**  
  - Stores and manages item slots  
  - Handles stacking, swapping, usage, notifications

- **UItemData (Primary Data Asset)**  
  - Icon, display name, max stack  
  - Linked behavior class for item usage

- **UItemUseBase (Polymorphic Item Behavior)**  
  - Each item triggers different actions  
  - Example: healing potion increases player HP

- **UInventorySlotWidget**
  - Individual slot UI logic  
  - Drag & Drop + highlight + quantity display

- **UInventoryWidget**
  - Creates & updates Hotbar & Backpack UI


## 📌 Technical Highlights

- Fully **modular** system for future expansion  
- Uses **event-driven updates** (`OnInventoryUpdated`)  
- Strong separation of **data vs. behavior**  
- Pickups created dynamically via item data  
- Clean UI code with BlueprintNativeEvents

---

## 🛠 Requirements

- Unreal Engine **5.5** or newer
- C++ project setup recommended
- Enhanced Input enabled

---
