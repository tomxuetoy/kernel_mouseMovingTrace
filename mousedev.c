static void mousedev_packet(struct mousedev_client *client,
  		    signed char *ps2_data)
{
	struct mousedev_motion *p = &client->packets[client->tail];
    int val;
    printk(KERN_WARNING "..................tomxue: case 19.................");
 
	ps2_data[0] = 0x08 |
		((p->dx < 0) << 4) | ((p->dy < 0) << 5) | (p->buttons & 0x07);
	ps2_data[1] = mousedev_limit_delta(p->dx, 127);
	ps2_data[2] = mousedev_limit_delta(p->dy, 127);
	p->dx -= ps2_data[1];
	p->dy -= ps2_data[2];
    val = p->dx;
    printk(KERN_WARNING "mousedev_packet: ps2_data[1]=%d ps2_data[2] =%d.................", ps2_data[1], ps2_data[2]);
    printk(KERN_WARNING "mousedev_packet: p->dx=%d p->dy=%d p=%p val=%d.................", p->dx, p->dy, p, val);
 
	switch (client->mode) {
	case MOUSEDEV_EMUL_EXPS:
		ps2_data[3] = mousedev_limit_delta(p->dz, 7);
		p->dz -= ps2_data[3];
		ps2_data[3] = (ps2_data[3] & 0x0f) | ((p->buttons & 0x18) << 1);
		client->bufsiz = 4;
		break;
 
	case MOUSEDEV_EMUL_IMPS:
		ps2_data[0] |=
			((p->buttons & 0x10) >> 3) | ((p->buttons & 0x08) >> 1);
		ps2_data[3] = mousedev_limit_delta(p->dz, 127);
		p->dz -= ps2_data[3];
		client->bufsiz = 4;
		break;
 
	case MOUSEDEV_EMUL_PS2:
	default:
		ps2_data[0] |=
			((p->buttons & 0x10) >> 3) | ((p->buttons & 0x08) >> 1);
		p->dz = 0;
		client->bufsiz = 3;
		break;
	}
 
	if (!p->dx && !p->dy && !p->dz) {
		if (client->tail == client->head) {
			client->ready = 0;
			client->last_buttons = p->buttons;
		} else
			client->tail = (client->tail + 1) % PACKET_QUEUE_LEN;
        printk(KERN_WARNING "client->tail=%d....................\n", client->tail);
	}
}
