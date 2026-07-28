#include "core/backend/FiberPool.hpp"
#include "core/backend/ScriptMgr.hpp"
#include "core/commands/Command.hpp"
#include "core/commands/LoopedCommand.hpp"
#include "core/commands/ListCommand.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/ScriptGlobal.hpp"
#include "game/gta/ScriptLocal.hpp"
#include "game/gta/Stats.hpp"
#include "game/pointers/Pointers.hpp"
#include "types/script/globals/GPBD_FM.hpp"
#include "types/script/globals/GPBD_FM_2.hpp"

namespace YimMenu::Features
{
	class ClaimNightclub : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.NightclubData.SafeCashValue > 0)
				*ScriptGlobal(2708832).As<BOOL*>() = TRUE;
		}
	};
	static ClaimNightclub _ClaimNightclub{"claimsafenightclub", "Nightclub", "Claim safe earnings from Nightclub."};

	class ClaimArcade : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.ArcadeData.SafeCashValue > 0)
				*ScriptGlobal(2708841).As<BOOL*>() = TRUE;
		}
	};
	static ClaimArcade _ClaimArcade{"claimsafearcade", "Arcade", "Claim safe earnings from Arcade."};

	class ClaimAgency : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.FixerHQData.SafeCashValue > 0)
				*ScriptGlobal(2708850).As<BOOL*>() = TRUE;
		}
	};
	static ClaimAgency _ClaimAgency{"claimsafeagency", "Agency", "Claim safe earnings from Agency."};

	class ClaimSalvageYard : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.SalvageYardData.TotalEarnings > 0)
				*ScriptGlobal(2708859).As<BOOL*>() = TRUE;
			Stats::SetPackedInt(51051, 100);
		}
	};
	static ClaimSalvageYard _ClaimSalvageYard{"claimsafesalvageyard", "Salvage Yard", "Claim safe earnings from Salvage Yard."};

	class ClaimBailOffice : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.BailShopData.SafeCashValue > 0)
				*ScriptGlobal(2708868).As<BOOL*>() = TRUE;
		}
	};
	static ClaimBailOffice _ClaimBailOffice{"claimsafebailoffice", "Bail Office", "Claim safe earnings from Bail Office."};

	class ClaimGarmentFactory : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.HackerDenData.SafeCashValue > 0)
				*ScriptGlobal(2708883).As<BOOL*>() = TRUE;
		}
	};
	static ClaimGarmentFactory _ClaimGarmentFactory{"claimsafegarmentfactory", "Garment Factory", "Claim safe earnings from Garment Factory."};

	class ClaimHandsOnCarWash : public Command
	{
		using Command::Command;

	public:
		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			if (GPBD_FM_2::Get()->Entries[Self::GetPlayer().GetId()].SYVehSaleData.HOWCData.SafeCashValue > 0)
				*ScriptGlobal(2708890).As<BOOL*>() = TRUE;
			Stats::SetPackedInt(24924, 0);
			Stats::SetPackedInt(24925, 0);
			Stats::SetPackedInt(24926, 0);
		}
	};
	static ClaimHandsOnCarWash _ClaimHandsOnCarWash{"claimsafehowc", "Hands On Car Wash", "Claim safe earnings from Hands on Car Wash."};

	class NCPay : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			FiberPool::Push([]() {
				int m_loop = static_cast<int>(std::floor(static_cast<float>(250000 - GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.NightclubData.SafeCashValue) / 50000.0f));
				while (m_loop >= 0)
				{
					if (m_loop == 0)
					{
						*ScriptGlobal(2708832).As<BOOL*>() = TRUE;
					}
					else
					{
						Stats::SetInt("MPX_CLUB_POPULARITY", 1000);
						Stats::SetInt("MPX_CLUB_PAY_TIME_LEFT", -1);
						ScriptMgr::Yield(5000ms);
					}
					m_loop--;
				}
			});
		}
	};
	static NCPay ncpay("ncpay", "Nightclub Pay", "Pays nightclub income");

	class Dre : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			Stats::SetInt("MPX_FIXER_STORY_BS", 4095);
		}
	};
	static Dre dre("dre", "Dr. Dre", "Prep all mission Dr. Dre");

	class Avi : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			Stats::SetInt("MPX_M25_AVI_MISSION_CURRENT", 4);
		}
	};
	static Avi avi("avi", "Avi", "Prep all mission Avi");

	class Oscar : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			Stats::SetInt("MPX_HACKER24_INST_BS", 2047);
		}
	};
	static Oscar oscar("oscar", "Oscar", "Prep all mission Oscar");

	class Bunker : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (auto thread = Scripts::FindScriptThread("gb_gunrunning"_J))
			{
				auto localVar = ScriptLocal(thread, 1275).At(774);
				if (localVar.CanAccess())
				{
					*localVar.As<int*>() = 0;
					Notifications::Show("BNTT", "Sell Bunker", NotificationType::Success);
				}
			}
		}
	};
	static Bunker bunker("bunker", "Sell Bunker", "Instant sell bunker");

	class Resupply : public Command
	{
		using Command::Command;

	public:
		virtual void OnCall() override
		{
			for (int b = 1; b < 8; b++)
			{
				*ScriptGlobal(1673820 + b).As<INT*>() = 1;
				ScriptMgr::Yield(1000ms);
			}
		}
	};
	static Resupply resupply("resupply", "Resupply", "Resupply all MC, bunker, acid lap");

	class Cooldown : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		void SetTime(int time)
		{
			ScriptGlobal cooldown(262145);
			if (*cooldown.At(19170).As<int*>() == time)
				return;
			static std::vector<std::pair<int, int>> cooldowns = {
			    {19170, 180000},
			    {19525, 1200000},
			    {19526, 1680000},
			    {19527, 2340000},
			    {19528, 2880000},
			    {22567, 18000},
			    {24118, 300000},
			    {24159, 300000},
			    {24160, 300000},
			    {15592, 300000},
			    {15593, 180000},
			    {22567, 18000}};
			for (auto& cd : cooldowns)
			{
				*cooldown.At(cd.first).As<int*>() = time > 0 ? time : cd.second;
			}
		}
		virtual void OnTick() override
		{
			SetTime(1000);
		}
		virtual void OnDisable() override
		{
			SetTime(0);
		}
	};
	static Cooldown cooldown("cooldown", "Remove CD", "Remove cooldown");

	class Spin : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		virtual void OnTick() override
		{
			*ScriptGlobal(262145).At(26855).As<int*>() = 9;
			*ScriptGlobal(262145).At(26856).As<bool*>() = TRUE;
		}
		virtual void OnDisable() override
		{
			*ScriptGlobal(262145).At(26855).As<int*>() = 1;
			*ScriptGlobal(262145).At(26856).As<bool*>() = FALSE;
		}
	};
	static Spin spin("spin", "Add Spin", "Add spin lucky wheel");

	class SellCeo : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (auto thread = Scripts::FindScriptThread("gb_contraband_sell"_J))
			{
				auto localVar = ScriptLocal(thread, 576).At(1);
				if (localVar.CanAccess())
				{
					*localVar.As<int*>() = 67230;
					Notifications::Show("BNTT", "Sell Special Cargo", NotificationType::Success);
				}
			}
			else
			{
				Notifications::Show("BNTT", "Go to warehouse and sell cargo", NotificationType::Error);
			}
		}
	};
	static SellCeo sellceo("sellceo", "Sell Cargo##ceo", "Instant sell CEO");

	class SellAir : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (auto thread = Scripts::FindScriptThread("gb_smuggler"_J))
			{
				auto localVar = ScriptLocal(thread, 1998);
				if (localVar.CanAccess())
				{
					*localVar.At(1035).As<int*>() = 0;
					*localVar.At(1078).As<int*>() = 1;
					Notifications::Show("BNTT", "Sell Air Cargo", NotificationType::Success);
				}
			}
			else
			{
				Notifications::Show("BNTT", "Go to hangar and sell cargo via air", NotificationType::Error);
			}
		}
	};
	static SellAir sellair("sellair", "Sell Cargo##air", "Instant sell Air");

	static std::vector<std::pair<int, const char*>> warehouse_goods = {
	    {-1, "Default"},
	    {0, "Medical Supplies"},
	    {1, "Tobacco & Alcohol"},
	    {2, "Art & Antiques"},
	    {3, "Electronic Goods"},
	    {4, "Weapons & Ammo"},
	    {5, "Narcotics"},
	    {6, "Gemstones"},
	    {7, "Animal Materials"},
	    {8, "Counterfeit Goods"},
	    {9, "Jewelry"},
	    {10, "Bullion"}};
	static ListCommand _whgoods{"whgoods", "Goods##warehouse", "Warehouse goods category", warehouse_goods, -1};

	class Ceo : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			FiberPool::Push([] {
				for (int c = 0; c <= 4; c++)
				{
					*ScriptGlobal(1882762 + 16).As<INT*>() = _whgoods.GetState();
					*ScriptGlobal(1882762 + 13).As<INT*>() = 111;
					Stats::SetPackedBool(32359 + c, true);
					ScriptMgr::Yield(5000ms);
				}
			});
		}
	};
	static Ceo ceo("ceo", "Source Cargo##CEO", "Bring the goods to the warehouse.");

	static std::vector<std::pair<int, const char*>> hangar_goods = {
	    {-1, "Default"},
	    {0, "Animal Materials"},
	    {1, "Art & Antiques"},
	    {2, "Chemicals"},
	    {3, "Counterfeit Goods"},
	    {4, "Jewelry & Gemstones"},
	    {5, "Medical Supplies"},
	    {6, "Narcotics"},
	    {7, "Tobacco & Alcohol"}};
	static ListCommand _hggoods{"hggoods", "Goods##hangar", "Hangar goods category", hangar_goods, -1};

	class Air : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			*ScriptGlobal(1882787 + 8).As<INT*>() = _hggoods.GetState();
			*ScriptGlobal(1882787 + 7).As<INT*>() = 50;
			Stats::SetPackedBool(36828, true);
		}
	};
	static Air air("air", "Source Cargo##Air", "Bring the goods to the hangar.");

	class Vincent : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			Stats::SetInt("MPX_SALV23_INST_PROG", 31);
		}
	};
	static Vincent vincent("vincent", "Vincent", "Set prep mission vincent");

	static std::vector<std::pair<int, const char*>> prizes = {
	    {0, "Clothing 1"},
	    {1, "2500 RP"},
	    {2, "$20000"},
	    {3, "10000 Chips"},
	    {4, "10% Discount Voucher"},
	    {5, "5000 RP"},
	    {6, "$30000"},
	    {7, "15000 Chips"},
	    {8, "Clothing 2"},
	    {9, "7500 RP"},
	    {10, "20000 Chips"},
	    {11, "Mystery Prize"},
	    {12, "Clothing 3"},
	    {13, "10000 RP"},
	    {14, "$40000"},
	    {15, "25000 Chips"},
	    {16, "Clothing 4"},
	    {17, "15000 RP"},
	    {18, "Podium Vehicle"},
	    {19, "$50000"}};
	static ListCommand _clws{"clws", "Prizes##clw", "Lucky wheel category", prizes, 18};

	class Clw : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			auto local_player = Player(static_cast<uint8_t>(0));
			if (auto thread = Scripts::FindScriptThread("casino_lucky_wheel"_J))
			{
				auto localVar = ScriptLocal(thread, 150 + 1);
				if (localVar.CanAccess())
				{
					*localVar.At(local_player.GetId() * 5).As<int*>() = _clws.GetState();
				}
			}
		}
	};
	static Clw clw("clw", "Set Prizes", "Set prizes lucky wheel.");

	class Ride : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			*ScriptGlobal(1972842).As<BOOL*>() = TRUE;
		}
	};
	static Ride _Ride("ride", "Prize Ride", "Claim Prize Ride.");

	static std::vector<std::pair<int, const char*>> hits = {
	    {0, "Tech Entrepreneur"},
	    {1, "Judge"},
	    {2, "Cofounder"},
	    {3, "Trolls"},
	    {4, "Dealers"},
	    {5, "CEO"},
	    {6, "Popstar"},
	    {7, "Hitmen"}};
	static ListCommand _hits{"hits", "PayPhone Hits", "PayPhone Hits", hits, 0};

	class Phonehit : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			*ScriptGlobal(2733138).At(3989).At(348).As<INT*>() = _hits.GetState();
		}
	};
	static Phonehit _Phonehit("phonehit", "Set##pph", "Set PayPhone Hits.");
}
