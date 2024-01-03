void main()
{
    // INIT ECONOMY --------------------------------------
    Hive ce = CreateHive();
    if (ce)
        ce.InitOffline();

    // DATE RESET AFTER ECONOMY INIT -------------------------
    int year, month, day, hour, minute;
    int reset_month = 9, reset_day = 20;
    GetGame().GetWorld().GetDate(year, month, day, hour, minute);

    if ((month == reset_month) && (day < reset_day))
    {
        GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
    }
    else
    {
        if ((month == reset_month + 1) && (day > reset_day))
        {
            GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
        }
        else
        {
            if ((month < reset_month) || (month > reset_month + 1))
            {
                GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
            }
        }
    }
}

class CustomMission : MissionServer
{
    override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
    {
        Entity playerEnt;
        playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");
        Class.CastTo(m_player, playerEnt);

        // Remove all default clothing
        m_player.RemoveAllItems();

        // Select player
        GetGame().SelectPlayer(identity, m_player);

        return m_player;
    }

    override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
    {
        // Remove all default clothing
        player.RemoveAllItems();

        EntityAI courierBag;
        ItemBase itemEnt;

        // Create CourierBag
        courierBag = player.GetInventory().CreateInInventory("CourierBag");

        // Put items inside the CourierBag
        if (courierBag)
        {
            itemEnt = courierBag.GetInventory().CreateInInventory("BandageDressing");
            SetRandomHealth(itemEnt);

            itemEnt = courierBag.GetInventory().CreateInInventory("SmallStone");
            SetRandomHealth(itemEnt);

            itemEnt = courierBag.GetInventory().CreateInInventory("Pot");
            SetRandomHealth(itemEnt);
        }
    }

    void SetRandomHealth(EntityAI itemEnt)
    {
        if (itemEnt)
        {
            float rndHlt = Math.RandomFloat(0.45, 0.65);
            itemEnt.SetHealth01("", "", rndHlt);
        }
    }

    // Update dogtag dynamically based on kills and playtime
    override void UpdatePlayersStats()
    {
        super.UpdatePlayersStats();

        array<Man> players = {};
        GetGame().GetPlayers(players);

        for (int i = 0; i < players.Count(); i++)
        {
            PlayerBase player;
            if (!PlayerBase.CastTo(player, players.Get(i)))
                continue;

            // Update player's dogtag based on kills and playtime
            float playtime = player.StatGet("playtime");
            float kills = player.StatGet("players_killed");

            if ((playtime >= 7200 || kills >= 1) && !player.HasDogtag())
            {
                // Assign Granite Dogtag if playtime is 2 hours or more, or kills are 1 or more
                player.ReplaceDogtag("Dogtag_Granite");
            }
            else if ((playtime >= 86400 || kills >= 5) && !player.HasDogtag())
            {
                // Assign Cobalt Dogtag if playtime is 24 hours or more, or kills are 5 or more
                player.ReplaceDogtag("Dogtag_Cobalt");
            }
        }
    }
};

Mission CreateCustomMission(string path)
{
    return new CustomMission();
}
