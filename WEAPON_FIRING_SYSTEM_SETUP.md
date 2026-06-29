# ALS Community - Weapon Firing System Setup Guide

## 📋 Что было добавлено

Полная система стрельбы для оружия M4A1 с интеграцией в ALS Character System.

### 📁 Созданные файлы:

```
Source/ALSV4_CPP/
├── Public/Weapon/
│   ├── ALSWeapon.h
│   └── ALSWeaponComponent.h
└── Private/Weapon/
    ├── ALSWeapon.cpp
    └── ALSWeaponComponent.cpp
```

## ⚙️ Функционал

✅ **Система стрельбы:**
- LineTrace для определения попаданий
- Урон при попадании в врагов (30 DMG)
- Автоматический огневой режим
- Кулдаун между выстрелами (600 выстр/мин)
- Debug визуализация (зелёные/красные линии)

✅ **Интеграция:**
- Автоматическое присоединение к персонажу
- Управление через Input System
- Поддержка HeldObject системы ALS

## 🚀 Инструкция по установке

### Шаг 1: Обновить Build.cs файл

Открыть `Source/ALSV4_CPP/ALSV4_CPP.Build.cs`

Убедитесь, что есть необходимые модули:
```csharp
PublicDependencyModuleNames.AddRange(new[]
{
    "Core", "CoreUObject", "Engine", "InputCore", 
    "NavigationSystem", "AIModule", "GameplayTasks",
    "PhysicsCore", "Niagara", "EnhancedInput"
});
```

### Шаг 2: Добавить компонент в персонажа

В Blueprint персонажа (BP_ALSCharacter или наследнике):

1. **Add Component** → **ALSWeaponComponent**
2. В Details установить:
   - **Default Weapon Class**: Выбрать/создать Blueprint оружия
   - **Weapon Attachment Bone**: `VB RHS_ik_hand_gun` (по умолчанию)

### Шаг 3: Настроить Input System

В **Project Settings → Input**:

1. Открыть **Input** секцию
2. Добавить новый **Action Mapping**:
   - Name: `Fire`
   - Key: `Left Mouse Button` (или другая кнопка)

### Шаг 4: Создать Blueprint оружия

1. **Content Browser** → Создать новый Blueprint
2. Parent Class: **ALSWeapon**
3. Назвать: **BP_M4A1**
4. Открыть и установить:

**В Mesh компоненте:**
- **Static Mesh**: Выбрать меш M4A1
- **Relative Location**: `(0, 0, 0)` или отрегулировать положение

**В Details (ALSWeapon секция):**
- **Muzzle Socket Name**: `Muzzle` (если сокет есть в меше)
- **Fire Rate**: `600` (выстрелов в минуту)
- **Base Damage**: `30` (урон за выстрел)
- **Trace Distance**: `5000` (дальность стрельбы в юнитах)

### Шаг 5: Перекомпилировать проект

1. **Visual Studio** → Правый клик на .sln → **Generate Visual Studio project files**
2. **Build Solution** (Ctrl+Shift+B)
3. Открыть проект в **Unreal Editor**
4. При появлении диалога - выбрать **Rebuild** для скомпилированных модулей

## 🎮 Как использовать

1. Запустить игру (Play в Editor)
2. Нажать **ЛКМ** (Left Mouse Button) чтобы стрелять
3. При попадании:
   - Зелёная линия = траектория пули
   - Красная сфера = точка попадания
   - Урон применяется врагам (если у них есть Health)

## 🔧 Параметры оружия

| Параметр | Значение | Описание |
|----------|----------|---------|
| FireRate | 600 | Выстрелов в минуту (0.1 сек между выстрелами) |
| BaseDamage | 30 | Урон за один выстрел |
| TraceDistance | 5000 | Максимальная дальность стрельбы в юнитах |
| MuzzleSocketName | "Muzzle" | Имя сокета для выхода пули |

## 📊 Debug информация

При стрельбе в Output Log появляются сообщения:

```
Hit: Enemy_01 - Damage: 30.00
```

Визуализация:
- 🟢 Зелёная линия - пуля попала в цель
- 🔴 Красная линия - пуля не попала
- 🔴 Красная сфера - точка попадания (5 юнитов в радиусе)

## 🐛 Troubleshooting

### Оружие не стреляет

1. ✅ Проверить, добавлен ли компонент **ALSWeaponComponent** к персонажу
2. ✅ Убедиться, что **Default Weapon Class** установлен
3. ✅ Проверить, работает ли Input Action **Fire**
4. ✅ Перекомпилировать C++ код

### Урон не наносится

1. ✅ Проверить, есть ли у цели компонент **CharacterMovement** или иные компоненты для урона
2. ✅ Убедиться, что враг наследует **AALSBaseCharacter** или имеет функцию `TakeDamage()`
3. ✅ Проверить дистанцию (не слишком далеко ли враг)

### Оружие не видно на персонаже

1. ✅ Проверить **Weapon Attachment Bone** - должен быть корректный сокет скелета
2. ✅ Убедиться, что Static Mesh установлен в Blueprint оружия
3. ✅ Проверить масштаб и позицию оружия

## 📈 Дальнейшие улучшения

- [ ] Визуальные эффекты (вспышка дула, дым)
- [ ] Звуковые эффекты выстрела
- [ ] Система боезапаса/патронов
- [ ] Разные типы оружия
- [ ] Отдача и реакойл при выстреле
- [ ] Прицел и первое-лицевой вид
- [ ] Перезарядка оружия
- [ ] Эффект оболочки после выстрела

## 🔗 Git команды

```bash
# Скачать ветку с системой стрельбы
git fetch origin
git checkout feature/weapon-firing-system

# Или обновить существующий checkout
git pull origin feature/weapon-firing-system

# Слить в main когда готово
git checkout main
git merge feature/weapon-firing-system
```

## 💡 Советы

1. **Первая компиляция** может занять время (5-10 минут)
2. **Если ошибки компиляции** - открыть Output Log и прочитать полные сообщения об ошибках
3. **Для боевых врагов** - добавить компонент Health и реакцию на урон
4. **Для тестирования** - добавить врагов из ALS примеров в уровень

## 📚 Документация

- [UE5 Damage System](https://docs.unrealengine.com/5.4/en-US/taking-damage-in-unreal-engine/)
- [Line Tracing](https://docs.unrealengine.com/5.4/en-US/line-tracing-with-unreal-engine/)
- [ALS Community GitHub](https://github.com/PanicPetal/ALS-Community)

---

**Автор**: ALS Community  
**Версия**: 1.0  
**Последнее обновление**: 2024  
**Лицензия**: Лицензия проекта ALS
