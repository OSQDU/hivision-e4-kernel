#include <linux/mii.h>
#include <linux/phy.h>
#include <linux/slab.h>
#include <mach/fh_gmac.h>
#include "fh_gmac_phyt.h"
#include "fh_gmac.h"

static int fh_mdio_read(struct mii_bus *bus, int phyaddr, int phyreg)
{
	struct net_device *ndev = bus->priv;
	Gmac_Object *pGmac = netdev_priv(ndev);

	if (phyaddr < 0)
		return -ENODEV;

	writel(phyaddr << 11 | gmac_gmii_clock_100_150 << 2 | phyreg << 6 | 0x1,
	       pGmac->remap_addr + REG_GMAC_GMII_ADDRESS);

	while (readl(pGmac->remap_addr + REG_GMAC_GMII_ADDRESS) & 0x1) {

	}

	return readl(pGmac->remap_addr + REG_GMAC_GMII_DATA);
}

static int fh_mdio_write(struct mii_bus *bus, int phyaddr, int phyreg,
			 u16 phydata)
{
	struct net_device *ndev = bus->priv;
	Gmac_Object *pGmac = netdev_priv(ndev);

	if (phyaddr < 0)
		return -ENODEV;

	writel(phydata, pGmac->remap_addr + REG_GMAC_GMII_DATA);
	writel(0x1 << 1 | phyaddr << 11 | gmac_gmii_clock_100_150 << 2 | phyreg
	       << 6 | 0x1, pGmac->remap_addr + REG_GMAC_GMII_ADDRESS);

	while (readl(pGmac->remap_addr + REG_GMAC_GMII_ADDRESS) & 0x1) {

	}
	return 0;
}

int fh_mdio_reset(struct mii_bus *bus)
{
    return 0;
}

int fh_mdio_set_mii(struct mii_bus *bus)
{
    struct net_device *ndev = bus->priv;
    Gmac_Object *pGmac = netdev_priv(ndev);
    __u32 rmii_mode;
    int phyid = pGmac->priv_data->phyid;
#ifdef CONFIG_FH_GMAC_PHY_RTL8201
    //switch to page7
    fh_mdio_write(bus, phyid, gmac_phyt_rtl8201_page_select, 7);
    //phy rmii register settings
    rmii_mode = fh_mdio_read(bus, phyid, gmac_phyt_rtl8201_rmii_mode);
#endif

#ifdef CONFIG_PHY_IP101G
    fh_mdio_write(bus, phyid, gmac_phyt_ip101g_page_select, 16);
    //gmac_phyt_rtl8201_rmii_mode == gmac_phyt_ip101g_rmii_mode
    rmii_mode = fh_mdio_read(bus, phyid, gmac_phyt_rtl8201_rmii_mode);
#endif

#ifdef CONFIG_FH_GMAC_PHY_TI83848
    rmii_mode = fh_mdio_read(bus, phyid, gmac_phyt_ti83848_rmii_mode);
#endif

    if (pGmac->phy_interface == PHY_INTERFACE_MODE_RMII) {
#ifdef CONFIG_FH_GMAC_PHY_RTL8201
        //0x7ffb when an external clock inputs to the CKXTAL2 pin
        rmii_mode = 0x7ffb;
#endif

#ifdef CONFIG_PHY_IP101G
        rmii_mode = 0x1006;
#endif

#ifdef CONFIG_FH_GMAC_PHY_TI83848
        rmii_mode |= 0x20;
#endif
    } else if (pGmac->phy_interface == PHY_INTERFACE_MODE_MII) {
#ifdef CONFIG_FH_GMAC_PHY_RTL8201
        rmii_mode = 0x6ff3;
#endif

#ifdef CONFIG_PHY_IP101G
        rmii_mode = 0x2;
#endif

#ifdef CONFIG_FH_GMAC_PHY_TI83848
        rmii_mode &= ~(0x20);
#endif
    }
#ifdef CONFIG_FH_GMAC_PHY_RTL8201
    fh_mdio_write(bus, phyid, gmac_phyt_rtl8201_rmii_mode, rmii_mode);

    //back to page0
    fh_mdio_write(bus, phyid, gmac_phyt_rtl8201_page_select, 0);
#endif

#ifdef CONFIG_PHY_IP101G
    fh_mdio_write(bus, phyid, gmac_phyt_rtl8201_rmii_mode, rmii_mode);
    fh_mdio_write(bus, phyid, gmac_phyt_ip101g_page_select, 0x10);
#endif

#ifdef CONFIG_FH_GMAC_PHY_TI83848
    fh_mdio_write(bus, phyid, gmac_phyt_ti83848_rmii_mode, rmii_mode);
#endif

    return 0;
}

int fh_mdio_register(struct net_device *ndev)
{
	int err = 0, found, addr;
	struct mii_bus *new_bus;
	Gmac_Object *pGmac = netdev_priv(ndev);

	new_bus = mdiobus_alloc();
	if (new_bus == NULL)
		return -ENOMEM;
	new_bus->name =
	    pGmac->phy_interface ==
	    PHY_INTERFACE_MODE_MII ? "gmac_mii" : "gmac_rmii";
	new_bus->read = &fh_mdio_read;
	new_bus->write = &fh_mdio_write;
	new_bus->reset = &fh_mdio_reset;
	snprintf(new_bus->id, MII_BUS_ID_SIZE, "%x", 0);
	new_bus->priv = ndev;
	new_bus->parent = pGmac->dev;
	err = mdiobus_register(new_bus);
	if (err != 0) {
		pr_err("%s: Cannot register as MDIO bus, error: %d\n",
		       new_bus->name, err);
		goto bus_register_fail;
	}

	pGmac->mii = new_bus;

	found = 0;
	for (addr = 0; addr < 32; addr++) {
		struct phy_device *phydev = new_bus->phy_map[addr];
		if (phydev) {
			if (pGmac->priv_data->phyid == -1) {
				pGmac->priv_data->phyid = addr;
			}
			pr_info("%s: PHY ID %08x at %d IRQ %d (%s)%s\n",
				ndev->name, phydev->phy_id, addr,
				phydev->irq, dev_name(&phydev->dev),
				(addr ==
				 pGmac->priv_data->phyid) ? " active" : "");
			found = 1;
			break;
		}
	}

	if (!found)
		pr_warning("%s: No PHY found\n", ndev->name);

	return 0;

bus_register_fail:
	kfree(new_bus);
	return err;
}

int fh_mdio_unregister(struct net_device *ndev)
{
	Gmac_Object *pGmac = netdev_priv(ndev);

	mdiobus_unregister(pGmac->mii);
	pGmac->mii->priv = NULL;
	kfree(pGmac->mii);
	return 0;
}
